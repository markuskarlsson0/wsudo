#include "app/backend.h"
#include "process/console.h"
#include "process/remote.h"
#include "utils/log.h"
#include <Windows.h>
#include <cstdlib>
#include <exception>
#include <format>
#include <functional>
#include <string>
#include <thread>

namespace app {

Backend::Backend(DWORD processId, const std::wstring& command) {
    process::Remote frontend(processId);
    frontend.open();
    frontend.connectToConsole();

    // Ignore all ctrl commands so that they can be forwarded to the console process
    process::Console::setCtrlHandler();

    std::thread thread(&Backend::wait, this, std::ref(frontend));
    thread.detach();

    std::wstring startCommand = std::format(L"cmd.exe /c {}", command);

    process::Console console(startCommand);
    console.start();
    console.wait();

    frontend.terminate();
}

void Backend::wait(process::Remote& remote) {
    try {
        remote.wait();
    } catch (const std::exception& exception) {
        utils::logException(exception);
    }

    std::exit(0);
}

} // namespace app
