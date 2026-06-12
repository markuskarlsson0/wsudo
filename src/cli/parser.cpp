#include "cli/parser.h"
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

std::string join(const std::vector<std::string>& arguments) {
    std::string combined;

    for (size_t i = 0; i < arguments.size(); i++) {
        if (i != 0) {
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

    if (argument0 == "-h" || argument0 == "--help") {
        arguments.help = true;
    } else {
        arguments.command = join(splitArguments);
    }

    return arguments;
}

} // namespace cli
