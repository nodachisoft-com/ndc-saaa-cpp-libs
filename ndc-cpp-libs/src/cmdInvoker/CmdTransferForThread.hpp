#pragma once
#include "cmd/CmdBase.hpp"
#include <deque>

class CmdTransferForThread
{
  std::deque<CmdBase *> cmdQueue;

public:
  CmdTransferForThread() : cmdQueue({}) {}
  void appendCmd(CmdBase *elem);
  CmdBase *readNextCmd();
};
