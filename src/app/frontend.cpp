#include "app/frontend.h"
#include "process/console.h"
#include "process/elevated.h"
#include "utils/exception.h"
#include <Windows.h>
#include <format>
#include <stdexcept>
#include <string>

namespace app {

Frontend::Frontend(const std::wstring& command) {
    // Ignore all ctrl commands so that they can be forwarded to the console process
    process::Console::setCtrlHandler();

    std::wstring filePath = getFilePath();
    DWORD processId = GetCurrentProcessId();

    std::wstring startCommand =
        std::format(L"--backend {} {}", processId, command.empty() ? L"cmd.exe /k" : command);

    process::Elevated backend(filePath, startCommand);
    bool result = backend.start();

    if (result) {
        backend.wait();
    }
}

std::wstring Frontend::getFilePath() {
    wchar_t filePath[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, filePath, MAX_PATH);

    if (length == 0) {
        throw utils::Exception("Failed to get file path", GetLastError());
    }

    if (length >= MAX_PATH) {
        throw std::runtime_error("File path is too long");
    }

    return filePath;
}

} // namespace app
