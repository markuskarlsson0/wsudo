#include "cli/parser.h"
#include <stdexcept>
#include <string>

namespace cli {

namespace {

bool isSpace(wchar_t character) { return character == L' ' || character == L'\t'; }

std::size_t findTokenEnd(const std::wstring& commandLine) {
    std::size_t position = 0;

    if (!commandLine.empty() && commandLine[0] == L'"') {
        position = commandLine.find(L'"', 1);
        return position == std::wstring::npos ? commandLine.size() : position + 1;
    }

    while (position < commandLine.size() && !isSpace(commandLine[position])) {
        position++;
    }

    return position;
}

std::wstring leadingToken(const std::wstring& commandLine) {
    std::wstring token = commandLine.substr(0, findTokenEnd(commandLine));

    if (token.size() >= 2 && token.front() == L'"' && token.back() == L'"') {
        return token.substr(1, token.size() - 2);
    }

    if (token.size() == 1 && token.front() == L'"') {
        return L"";
    }

    return token;
}

std::wstring stripLeadingToken(const std::wstring& commandLine) {
    std::size_t position = findTokenEnd(commandLine);

    while (position < commandLine.size() && isSpace(commandLine[position])) {
        position++;
    }

    return commandLine.substr(position);
}

} // namespace

Arguments parse(const std::wstring& commandLine, bool debug) {
    Arguments arguments;
    std::wstring rest = stripLeadingToken(commandLine);
    rest = debug ? stripLeadingToken(rest) : rest;

    std::wstring argument0 = leadingToken(rest);
    bool onlyArgument = stripLeadingToken(rest).empty();
    std::wstring backendPipePrefix = LR"(\\.\pipe\wsudo\)";

    if (onlyArgument && argument0.starts_with(backendPipePrefix)) {
        if (argument0.size() == backendPipePrefix.size()) {
            throw std::runtime_error("Invalid pipe name");
        }

        arguments.pipeName = argument0;
        arguments.backend = true;
    } else if (argument0 == L"-h" || argument0 == L"--help") {
        arguments.help = true;
    } else if (argument0 == L"-v" || argument0 == L"--version") {
        arguments.version = true;
    } else {
        arguments.command = rest;
    }

    return arguments;
}

} // namespace cli
