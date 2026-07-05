#pragma once

#include "process/process.h"
#include "utils/handle.h"
#include <string>

namespace process {

class Console : public Process {
  public:
    Console(const std::wstring& command);
    static void setCtrlHandler();
    void start();

  private:
    void createJob();
    void configureJob();
    void createProcess();
    void assignProcessToJob();
    void closeHandles() override;

    utils::Handle jobHandle_;
    utils::Handle threadHandle_;
    std::wstring command_;
};

} // namespace process
