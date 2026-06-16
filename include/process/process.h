#pragma once

#include "utils/handle.h"

namespace process {

class Process {
  public:
    void wait();
    void terminate();

  protected:
    virtual void closeHandles();

    utils::Handle processHandle_;
};

} // namespace process
