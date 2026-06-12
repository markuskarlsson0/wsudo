#include "cli/version.h"
#include <format>
#include <iostream>
#include <string_view>

namespace cli {

void version() {
    std::string_view version =
        std::string_view(WSUDO_VERSION_STRING).empty() ? "unknown" : WSUDO_VERSION_STRING;

    std::string_view arch =
        std::string_view(WSUDO_TARGET_ARCH).empty() ? "unknown" : WSUDO_TARGET_ARCH;

    std::cout << std::format("wsudo version {} ({})\n", version, arch);
}

} // namespace cli
