#include "app/frontend.h"
#include "ipc/data/data.h"
#include "ipc/pipe/host.h"
#include "process/console.h"
#include "process/elevated.h"
#include "utils/exception.h"
#include <Windows.h>
#include <stdexcept>
#include <string>

namespace app {

Frontend::Frontend(const std::wstring& command, bool debug) {
    // Ignore all ctrl commands so that they can be forwarded to the console process
    process::Console::setCtrlHandler();

    std::wstring filePath = getFilePath();
    DWORD processId = GetCurrentProcessId();
    std::wstring pipeName;
    ipc::pipe::Host pipe(pipeName);

    std::wstring parameters = debug ? L"--debug " + pipeName : pipeName;

    process::Elevated backend(filePath, parameters);
    bool result = backend.start();

    if (result) {
        pipe.waitForConnection();
        pipe.send(ipc::data::Data(processId, command));
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
