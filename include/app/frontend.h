#pragma once

#include <string>

namespace app {

class Frontend {
  public:
    Frontend(const std::wstring& command, bool debug = false);

  private:
    std::wstring getFilePath();
};

} // namespace app
