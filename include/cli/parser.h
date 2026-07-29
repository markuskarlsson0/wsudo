#pragma once

#include <string>

namespace cli {

struct Arguments {
    std::wstring command;
    std::wstring pipeName;
    bool backend = false;
    bool help = false;
    bool version = false;
};

Arguments parse(int argc, wchar_t* argv[]);

} // namespace cli
