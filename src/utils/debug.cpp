#include "utils/debug.h"
#include <Windows.h>
#include <string_view>

namespace utils {

void waitForDebugger() {
    while (!IsDebuggerPresent()) {
        Sleep(100);
    }

    DebugBreak();
}

bool checkDebugFlag([[maybe_unused]] int argc, [[maybe_unused]] wchar_t** argv) {
#ifdef WSUDO_DEBUG_BUILD
    return argc > 1 && std::wstring_view(argv[1]) == L"--debug";
#else
    return false;
#endif
}

} // namespace utils
