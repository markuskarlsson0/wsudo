#pragma once

#include <string>

namespace cli {

struct Arguments {
    std::string command;
};

Arguments parse(int argc, char* argv[]);

} // namespace cli
