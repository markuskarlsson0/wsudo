#pragma once

#include "process/remote.h"
#include <Windows.h>
#include <string>

namespace app {

class Backend {
  public:
    Backend(DWORD processId, const std::string& command);

  private:
    void wait(process::Remote& remote);
};

} // namespace app
