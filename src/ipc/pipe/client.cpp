#include "ipc/pipe/client.h"
#include "utils/exception.h"
#include <Windows.h>
#include <stdexcept>
#include <string>

namespace ipc::pipe {

namespace {

constexpr DWORD openTimeoutMs = 30000;
constexpr DWORD openRetryDelayMs = 100;

} // namespace

Client::Client(const std::wstring& pipeName) { open(pipeName); }

void Client::open(const std::wstring& pipeName) {
    DWORD64 start = GetTickCount64();

    while (GetTickCount64() - start < openTimeoutMs) {
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

    throw std::runtime_error("Timed out waiting for pipe");
}

} // namespace ipc::pipe
