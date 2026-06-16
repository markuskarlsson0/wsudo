#include "process/console.h"
#include "utils/exception.h"
#include <Windows.h>
#include <string>

namespace process {

Console::Console(const std::string& command) : command_(command) {
    createJob();
    configureJob();
    createProcess();
    assignProcessToJob();
}

void Console::setCtrlHandler() {
    // Ignore all ctrl commands
    BOOL result = SetConsoleCtrlHandler([](DWORD) { return TRUE; }, TRUE);

    if (!result) {
        throw utils::Exception("Failed to set console ctrl handler", GetLastError());
    }
}

void Console::start() {
    DWORD result = ResumeThread(threadHandle_);

    if (result == static_cast<DWORD>(-1)) {
        terminate();
        throw utils::Exception("Failed to resume thread", GetLastError());
    }
}

void Console::createJob() {
    jobHandle_ = CreateJobObjectA(NULL, NULL);

    if (!jobHandle_) {
        throw utils::Exception("Failed to create job object", GetLastError());
    }
}

void Console::configureJob() {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

    BOOL result =
        SetInformationJobObject(jobHandle_, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

    if (!result) {
        throw utils::Exception("Failed to set job object information", GetLastError());
    }
}

void Console::createProcess() {
    STARTUPINFOA si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {};

    BOOL result = CreateProcessA(NULL, command_.data(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL,
                                 NULL, &si, &pi);

    if (!result) {
        throw utils::Exception("Failed to create process", GetLastError());
    }

    processHandle_ = pi.hProcess;
    threadHandle_ = pi.hThread;
}

void Console::assignProcessToJob() {
    BOOL result = AssignProcessToJobObject(jobHandle_, processHandle_);

    if (!result) {
        terminate();
        throw utils::Exception("Failed to assign process to job object", GetLastError());
    }
}

void Console::closeHandles() {
    processHandle_.reset();
    threadHandle_.reset();
    jobHandle_.reset();
}

} // namespace process
