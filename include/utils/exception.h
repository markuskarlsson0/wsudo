#pragma once

#include <Windows.h>
#include <system_error>

namespace utils {

class Exception : public std::system_error {
  public:
    Exception(const char* message, DWORD code = 0);
};

} // namespace utils
