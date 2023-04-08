#include <memory>
#include "CmdTransferForThread.hpp"

using namespace nl;

void CmdTransferForThread::appendCmd(std::unique_ptr<CmdBase> elem)
{
  cmdQueue.push_back(std::move(elem));
}

std::unique_ptr<CmdBase> CmdTransferForThread::readNextCmd()
{
  if (cmdQueue.empty()) {
    return nullptr;
  }
  std::unique_ptr<CmdBase> result = std::move(cmdQueue.front());
  cmdQueue.pop_front();
  return result;

}
