#pragma once

#include <Windows.h>
#include <string>

namespace cli {

struct Arguments {
    std::wstring command;
    DWORD processId;
    bool backend = false;
    bool help = false;
    bool version = false;
};

Arguments parse(int argc, wchar_t* argv[]);

} // namespace cli
