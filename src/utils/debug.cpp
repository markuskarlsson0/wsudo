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

bool consumeDebugFlag([[maybe_unused]] int& argc, [[maybe_unused]] wchar_t**& argv) {
#ifdef WSUDO_DEBUG_BUILD
    if (argc > 1 && std::wstring_view(argv[1]) == L"--debug") {
        argc -= 1;
        argv += 1;
        return true;
    }
#endif

    return false;
}

} // namespace utils
