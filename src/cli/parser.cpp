#include "cli/parser.h"
#include <Windows.h>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace cli {

namespace {

std::vector<std::wstring> split(int argc, wchar_t* argv[]) {
    std::vector<std::wstring> arguments;

    for (int i = 1; i < argc; i++) {
        arguments.push_back(argv[i]);
    }

    return arguments;
}

std::wstring join(const std::vector<std::wstring>& arguments, std::size_t startOffset = 0) {
    std::wstring combined;

    for (std::size_t i = startOffset; i < arguments.size(); i++) {
        if (i != startOffset) {
            combined += L" ";
        }

        combined += arguments[i];
    }

    return combined;
}

} // namespace

Arguments parse(int argc, wchar_t* argv[]) {
    Arguments arguments;
    std::vector<std::wstring> splitArguments = split(argc, argv);
    std::wstring argument0 = splitArguments.empty() ? L"" : splitArguments[0];

    if (argument0 == L"--backend") {
        if (splitArguments.size() < 3 || splitArguments[1].empty() || splitArguments[2].empty()) {
            throw std::runtime_error("Invalid backend arguments");
        }

        arguments.command = join(splitArguments, 2);
        arguments.processId = static_cast<DWORD>(std::stoul(splitArguments[1]));
        arguments.backend = true;
    } else if (argument0 == L"-h" || argument0 == L"--help") {
        arguments.help = true;
    } else if (argument0 == L"-v" || argument0 == L"--version") {
        arguments.version = true;
    } else {
        arguments.command = join(splitArguments);
    }

    return arguments;
}

} // namespace cli
