#include "process/elevated.h"
#include <Windows.h>
#include <string>

namespace process {

Elevated::Elevated(const std::string& filePath, const std::string& command)
    : filePath_(filePath), command_(command) {}

bool Elevated::start() {
    SHELLEXECUTEINFOA sei = {};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "runas";
    sei.lpFile = filePath_.c_str();
    sei.lpParameters = command_.c_str();
    sei.nShow = SW_HIDE;

    BOOL result = ShellExecuteExA(&sei);

    if (!result) {
        return false;
    }

    processHandle_ = sei.hProcess;
    return true;
}

} // namespace process
