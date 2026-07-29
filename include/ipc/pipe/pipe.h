#pragma once

#include "ipc/data/data.h"
#include "utils/handle.h"
#include <Windows.h>
#include <functional>

namespace ipc::pipe {

class Pipe {
  public:
    void send(const data::Data& data);
    data::Data receive();

  protected:
    Pipe() = default;

    utils::Handle pipe_;

  private:
    void io(DWORD expectedBytes,
            std::function<BOOL(DWORD& bytes, OVERLAPPED& overlapped)> operation);
};

} // namespace ipc::pipe
