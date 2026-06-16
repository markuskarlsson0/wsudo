#include "process/remote.h"
#include "utils/exception.h"
#include <Windows.h>

namespace process {

Remote::Remote(DWORD processId) : processId_(processId) {}

void Remote::open() {
    processHandle_ = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, processId_);

    if (!processHandle_) {
        throw utils::Exception("Failed to open process", GetLastError());
    }
}

void Remote::connectToConsole() {
    freeConsole();
    attachConsole();
}

void Remote::freeConsole() {
    BOOL result = FreeConsole();

    if (!result) {
        throw utils::Exception("Failed to free console", GetLastError());
    }
}

void Remote::attachConsole() {
    BOOL result = AttachConsole(processId_);

    if (!result) {
        throw utils::Exception("Failed to attach console", GetLastError());
    }
}

} // namespace process
