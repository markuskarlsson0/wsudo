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

Arguments parse(const std::wstring& commandLine, bool debug = false);

} // namespace cli
