#include "ipc/data/data.h"
#include <stdexcept>

namespace ipc::data {

Data::Data(DWORD processId, const std::wstring& command) : processId(processId) {
    if (command.size() >= commandSize) {
        throw std::runtime_error("Command is too long");
    }

    wcscpy_s(this->command, commandSize, command.c_str());
}

} // namespace ipc::data
