#include "utils/log.h"
#include <Windows.h>
#include <fstream>
#include <string>

namespace utils {

void logException(const std::exception& exception) {
    char path[MAX_PATH];
    DWORD length = GetTempPathA(MAX_PATH, path);

    if (length > 0 && length < MAX_PATH) {
        std::ofstream(std::string(path) + "wsudo.log", std::ios::app)
            << exception.what() << std::endl;
    }
}

} // namespace utils
