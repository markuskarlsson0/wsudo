#include "ipc/pipe/client.h"
#include "utils/exception.h"
#include <Windows.h>
#include <string>

namespace ipc::pipe {

namespace {

constexpr DWORD openRetryDelayMs = 100;

} // namespace

Client::Client(const std::wstring& pipeName) { open(pipeName); }

void Client::open(const std::wstring& pipeName) {
    while (true) {
        pipe_ = CreateFileW(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED, NULL);

        if (pipe_ == INVALID_HANDLE_VALUE) {
            DWORD error = GetLastError();

            if (error == ERROR_PIPE_BUSY) {
                if (!WaitNamedPipeW(pipeName.c_str(), openRetryDelayMs)) {
                    DWORD waitError = GetLastError();

                    if (waitError != ERROR_SEM_TIMEOUT) {
                        throw utils::Exception("Failed to wait for pipe", waitError);
                    }
                }
            } else if (error == ERROR_FILE_NOT_FOUND) {
                Sleep(openRetryDelayMs);
            } else {
                throw utils::Exception("Failed to open pipe", error);
            }
        } else {
            return;
        }
    }
}

} // namespace ipc::pipe
