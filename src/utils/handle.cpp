#include "utils/handle.h"
#include <Windows.h>

namespace utils {

Handle::Handle() noexcept : handle_(NULL) {}

Handle::Handle(HANDLE handle) noexcept : handle_(handle) {}

Handle::~Handle() noexcept { close(); }

Handle::Handle(Handle&& handle) noexcept : handle_(handle.release()) {}

Handle& Handle::operator=(Handle&& handle) noexcept {
    if (this != &handle) {
        reset(handle.release());
    }

    return *this;
}

HANDLE Handle::get() const noexcept { return handle_; }

Handle::operator HANDLE() const noexcept { return handle_; }

Handle::operator bool() const noexcept {
    return handle_ != NULL && handle_ != INVALID_HANDLE_VALUE;
}

HANDLE Handle::release() noexcept {
    HANDLE handle = handle_;
    handle_ = NULL;
    return handle;
}

void Handle::reset(HANDLE handle) noexcept {
    if (handle == INVALID_HANDLE_VALUE) {
        handle = NULL;
    }

    if (handle_ != handle) {
        close();
        handle_ = handle;
    }
}

Handle& Handle::operator=(HANDLE handle) noexcept {
    reset(handle);
    return *this;
}

void Handle::close() noexcept {
    if (handle_ != NULL && handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(handle_);
        handle_ = NULL;
    }
}

} // namespace utils
