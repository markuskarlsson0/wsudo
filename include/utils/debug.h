#pragma once

namespace utils {

void waitForDebugger();
bool consumeDebugFlag(int& argc, wchar_t**& argv);

} // namespace utils
