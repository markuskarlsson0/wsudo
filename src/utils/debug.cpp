#include "utils/debug.h"
#include <Windows.h>

namespace utils {

void waitForDebugger() {
    while (!IsDebuggerPresent()) {
        Sleep(100);
    }

    DebugBreak();
}

} // namespace utils
