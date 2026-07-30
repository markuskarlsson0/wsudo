#include "ipc/pipe/host.h"
#include "ipc/data/data.h"
#include "utils/exception.h"
#include <Windows.h>
#include <format>
#include <random>
#include <stdexcept>
#include <string>

namespace ipc::pipe {

namespace {

constexpr int createPipeAttempts = 5;

} // namespace

Host::Host(std::wstring& pipeName) { pipeName = create(); }

void Host::waitForConnection() {
    OVERLAPPED overlapped{};
    utils::Handle eventHandle(CreateEvent(NULL, TRUE, FALSE, NULL));
    overlapped.hEvent = eventHandle;

    if (!overlapped.hEvent) {
        throw utils::Exception("Failed to create event", GetLastError());
    }

    if (!ConnectNamedPipe(pipe_, &overlapped)) {
        DWORD error = GetLastError();

        if (error == ERROR_IO_PENDING) {
            DWORD bytes = 0;
            DWORD waitResult = WaitForSingleObject(overlapped.hEvent, INFINITE);

            if (waitResult != WAIT_OBJECT_0) {
                throw utils::Exception("Failed to wait for pipe connection", GetLastError());
            }

            BOOL result = GetOverlappedResult(pipe_, &overlapped, &bytes, FALSE);

            if (!result) {
                throw utils::Exception("Failed to wait for pipe connection", GetLastError());
            }
        } else if (error != ERROR_PIPE_CONNECTED) {
            throw utils::Exception("Failed to wait for pipe connection", error);
        }
    }
}

std::wstring Host::createName() {
    static std::random_device randomDevice;
    unsigned long long suffix =
        (static_cast<unsigned long long>(randomDevice()) << 32) | randomDevice();

    return std::format(LR"(\\.\pipe\wsudo\{:016x})", suffix);
}

std::wstring Host::create() {
    for (int i = 0; i < createPipeAttempts; i++) {
        std::wstring pipeName = createName();
        HANDLE pipeHandle = CreateNamedPipeW(pipeName.c_str(),
                                             PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED |
                                                 FILE_FLAG_FIRST_PIPE_INSTANCE,
                                             PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1,
                                             ipc::data::dataSize, ipc::data::dataSize, 0, NULL);

        if (pipeHandle == INVALID_HANDLE_VALUE) {
            DWORD error = GetLastError();

            if (error != ERROR_ACCESS_DENIED && error != ERROR_PIPE_BUSY) {
                throw utils::Exception("Failed to create pipe", error);
            }
        } else {
            pipe_ = pipeHandle;
            return pipeName;
        }
    }

    throw std::runtime_error("Failed to create pipe");
}

} // namespace ipc::pipe
