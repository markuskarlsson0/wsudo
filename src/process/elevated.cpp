#include "process/elevated.h"
#include <Windows.h>
#include <string>

namespace process {

Elevated::Elevated(const std::wstring& filePath, const std::wstring& command)
    : filePath_(filePath), command_(command) {}

bool Elevated::start() {
    SHELLEXECUTEINFOW sei = {};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = L"runas";
    sei.lpFile = filePath_.c_str();
    sei.lpParameters = command_.c_str();
    sei.nShow = SW_HIDE;

    BOOL result = ShellExecuteExW(&sei);

    if (!result) {
        return false;
    }

    processHandle_ = sei.hProcess;
    return true;
}

} // namespace process
