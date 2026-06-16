#include "utils/exception.h"
#include <Windows.h>
#include <string>
#include <system_error>

namespace utils {

Exception::Exception(const char* message, DWORD code)
    : std::system_error(static_cast<int>(code), std::system_category(),
                        std::string(message) + " (" + std::to_string(code) + ")") {}

} // namespace utils
