#include "cli/help.h"
#include <iostream>

namespace cli {

void help() {
    std::cout << "Usage: wsudo [options] {command}\n\n"
              << "Options:\n"
              << "  -h, --help      Display this help message\n"
              << "  -v, --version   Display version information\n";
}

} // namespace cli
