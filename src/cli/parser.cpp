#include "cli/parser.h"
#include <Windows.h>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace cli {

namespace {

std::vector<std::string> split(int argc, char* argv[]) {
    std::vector<std::string> arguments;

    for (int i = 1; i < argc; i++) {
        arguments.push_back(argv[i]);
    }

    return arguments;
}

std::string join(const std::vector<std::string>& arguments, std::size_t startOffset = 0) {
    std::string combined;

    for (std::size_t i = startOffset; i < arguments.size(); i++) {
        if (i != startOffset) {
            combined += " ";
        }

        combined += arguments[i];
    }

    return combined;
}

} // namespace

Arguments parse(int argc, char* argv[]) {
    Arguments arguments;
    std::vector<std::string> splitArguments = split(argc, argv);
    std::string argument0 = splitArguments.empty() ? "" : splitArguments[0];

    if (argument0 == "--backend") {
        if (splitArguments.size() < 3 || splitArguments[1].empty() || splitArguments[2].empty()) {
            throw std::runtime_error("Invalid backend arguments");
        }

        arguments.command = join(splitArguments, 2);
        arguments.processId = static_cast<DWORD>(std::stoul(splitArguments[1]));
        arguments.backend = true;
    } else if (argument0 == "-h" || argument0 == "--help") {
        arguments.help = true;
    } else if (argument0 == "-v" || argument0 == "--version") {
        arguments.version = true;
    } else {
        arguments.command = join(splitArguments);
    }

    return arguments;
}

} // namespace cli
