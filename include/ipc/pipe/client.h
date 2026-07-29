#pragma once

#include "ipc/pipe/pipe.h"
#include <string>

namespace ipc::pipe {

class Client : public ipc::pipe::Pipe {
  public:
    Client(const std::wstring& pipeName);

  private:
    void open(const std::wstring& pipeName);
};

} // namespace ipc::pipe
