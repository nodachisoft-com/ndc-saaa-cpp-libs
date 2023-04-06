#pragma once
#include <deque>
#include "cmd/CmdBase.hpp"

class CmdTransferForThread
{
  std::deque<std::unique_ptr<CmdBase>> cmdQueue;

public:
  CmdTransferForThread() : cmdQueue() {}
  void appendCmd(std::unique_ptr<CmdBase> elem);
  std::unique_ptr<CmdBase> readNextCmd();
};
