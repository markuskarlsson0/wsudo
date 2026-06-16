#pragma once

#include <string>

namespace app {

class Frontend {
  public:
    Frontend(const std::string& command);

  private:
    std::string getFilePath();
};

} // namespace app
