#pragma once

#include <Windows.h>

namespace utils {

class Handle {
  public:
    Handle() noexcept;
    explicit Handle(HANDLE handle) noexcept;
    ~Handle() noexcept;
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    Handle(Handle&& other) noexcept;
    Handle& operator=(Handle&& other) noexcept;
    HANDLE get() const noexcept;
    operator HANDLE() const noexcept;
    explicit operator bool() const noexcept;
    HANDLE release() noexcept;
    void reset(HANDLE handle = NULL) noexcept;
    Handle& operator=(HANDLE handle) noexcept;

  private:
    void close() noexcept;

    HANDLE handle_;
};

} // namespace utils
