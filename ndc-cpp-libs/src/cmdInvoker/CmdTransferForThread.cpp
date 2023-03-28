#include "CmdTransferForThread.hpp"

void CmdTransferForThread::appendCmd(CmdBase *elem)
{
  cmdQueue.push_back(elem);
}

CmdBase *CmdTransferForThread::readNextCmd()
{
  std::size_t size = cmdQueue.size();
  if (size > 0)
  {
    CmdBase *result = cmdQueue[0];
    cmdQueue.pop_front();
    return result;
  }
  return nullptr;
}
