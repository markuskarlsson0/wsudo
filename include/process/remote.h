#pragma once

#include "process/process.h"
#include <Windows.h>

namespace process {

class Remote : public Process {
  public:
    Remote(DWORD processId);
    void open();
    void connectToConsole();

  private:
    void freeConsole();
    void attachConsole();

    DWORD processId_;
};

} // namespace process
