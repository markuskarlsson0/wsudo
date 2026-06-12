#pragma once

#include <string>

namespace cli {

struct Arguments {
    std::string command;
    bool help = false;
    bool version = false;
};

Arguments parse(int argc, char* argv[]);

} // namespace cli
