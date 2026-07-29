#pragma once

#include "process/remote.h"
#include <string>

namespace app {

class Backend {
  public:
    Backend(const std::wstring& pipeName);

  private:
    std::wstring createCommand(const wchar_t* value);
    void wait(process::Remote& remote);
};

} // namespace app
