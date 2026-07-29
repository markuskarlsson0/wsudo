#include "app/backend.h"
#include "ipc/data/data.h"
#include "ipc/pipe/client.h"
#include "process/console.h"
#include "process/remote.h"
#include "utils/log.h"
#include <Windows.h>
#include <cstdlib>
#include <exception>
#include <functional>
#include <string>
#include <thread>

namespace app {

Backend::Backend(const std::wstring& pipeName) {
    ipc::pipe::Client pipe(pipeName);
    ipc::data::Data data = pipe.receive();

    process::Remote frontend(data.processId);
    frontend.open();
    frontend.connectToConsole();

    // Ignore all ctrl commands so that they can be forwarded to the console process
    process::Console::setCtrlHandler();

    std::thread thread(&Backend::wait, this, std::ref(frontend));
    thread.detach();

    std::wstring command = createCommand(data.command);

    process::Console console(command);
    console.start();
    console.wait();

    frontend.terminate();
}

std::wstring Backend::createCommand(const wchar_t* value) {
    bool empty = value[0] == L'\0';
    std::wstring command = L"cmd.exe /";
    command += empty ? L"k" : L"c ";
    command += value;
    return command;
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
