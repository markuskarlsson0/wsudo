#include "ipc/pipe/pipe.h"
#include "ipc/data/data.h"
#include "utils/exception.h"
#include <Windows.h>
#include <functional>
#include <stdexcept>

namespace ipc::pipe {

void Pipe::send(const data::Data& data) {
    io(data::dataSize, [pipeHandle = HANDLE(pipe_), &data](DWORD& bytes, OVERLAPPED& overlapped) {
        return WriteFile(pipeHandle, &data, data::dataSize, &bytes, &overlapped);
    });
}

data::Data Pipe::receive() {
    data::Data data;

    io(data::dataSize, [pipeHandle = HANDLE(pipe_), &data](DWORD& bytes, OVERLAPPED& overlapped) {
        return ReadFile(pipeHandle, &data, data::dataSize, &bytes, &overlapped);
    });

    return data;
}

void Pipe::io(DWORD expectedBytes,
              std::function<BOOL(DWORD& bytes, OVERLAPPED& overlapped)> operation) {
    DWORD bytes = 0;
    OVERLAPPED overlapped{};

    utils::Handle eventHandle(CreateEvent(NULL, TRUE, FALSE, NULL));
    overlapped.hEvent = eventHandle;

    if (!overlapped.hEvent) {
        throw utils::Exception("Failed to create event", GetLastError());
    }

    BOOL result = operation(bytes, overlapped);

    if (!result) {
        DWORD error = GetLastError();

        if (error == ERROR_IO_PENDING) {
            DWORD waitResult = WaitForSingleObject(overlapped.hEvent, INFINITE);

            if (waitResult != WAIT_OBJECT_0) {
                throw utils::Exception("Failed to wait for pipe operation", GetLastError());
            }

            result = GetOverlappedResult(pipe_, &overlapped, &bytes, FALSE);

            if (!result) {
                throw utils::Exception("Failed with pipe operation", GetLastError());
            }
        } else {
            throw utils::Exception("Failed with pipe operation", error);
        }
    }

    if (bytes != expectedBytes) {
        throw std::runtime_error("Failed with pipe operation");
    }
}

} // namespace ipc::pipe
