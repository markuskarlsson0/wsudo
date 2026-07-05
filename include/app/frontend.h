#pragma once

#include <string>

namespace app {

class Frontend {
  public:
    Frontend(const std::wstring& command);

  private:
    std::wstring getFilePath();
};

} // namespace app
