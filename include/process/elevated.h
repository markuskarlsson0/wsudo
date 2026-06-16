#pragma once

#include "process/process.h"
#include <string>

namespace process {

class Elevated : public Process {
  public:
    Elevated(const std::string& filePath, const std::string& command);
    bool start();

  private:
    std::string filePath_;
    std::string command_;
};

} // namespace process
