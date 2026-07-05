#pragma once

#include "process/process.h"
#include <string>

namespace process {

class Elevated : public Process {
  public:
    Elevated(const std::wstring& filePath, const std::wstring& command);
    bool start();

  private:
    std::wstring filePath_;
    std::wstring command_;
};

} // namespace process
