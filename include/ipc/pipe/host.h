#pragma once

#include "ipc/pipe/pipe.h"
#include <string>

namespace ipc::pipe {

class Host : public ipc::pipe::Pipe {
  public:
    Host(std::wstring& pipeName);
    void waitForConnection();

  private:
    std::wstring createName();
    std::wstring create();
};

} // namespace ipc::pipe
