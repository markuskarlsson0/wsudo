#include "process/process.h"
#include "utils/exception.h"
#include <Windows.h>

namespace process {

void Process::wait() {
    DWORD result = WaitForSingleObject(processHandle_, INFINITE);

    if (result != WAIT_OBJECT_0) {
        throw utils::Exception("Failed to wait for process", GetLastError());
    }
}

void Process::terminate() {
    BOOL result = TerminateProcess(processHandle_, 0);

    if (!result) {
        throw utils::Exception("Failed to terminate process", GetLastError());
    }

    closeHandles();
};

void Process::closeHandles() { processHandle_.reset(); }

} // namespace process
