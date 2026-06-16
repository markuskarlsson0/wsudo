#pragma once

#include <Windows.h>
#include <string>

namespace cli {

struct Arguments {
    std::string command;
    DWORD processId;
    bool backend = false;
    bool help = false;
    bool version = false;
};

Arguments parse(int argc, char* argv[]);

} // namespace cli
