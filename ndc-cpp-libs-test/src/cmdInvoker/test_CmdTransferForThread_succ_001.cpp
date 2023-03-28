#include <string>
#include "../../pch.h"


using namespace nl;
using namespace std::literals::string_literals;

// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferForThread, append_readNext_case1)
{
  // From MainUI
  CmdTransferForThread cmdTransfer;
  cmdTransfer.appendCmd(new ExitForceCmd());

  // Brain UI cmdInvoker シミュレータ
  while (true)
  {
    CmdBase *cmd = cmdTransfer.readNextCmd();
    if (cmd == nullptr)
    {
      // コマンドがなくなったので終了
      break;
    }
    cmd->_EntryExec();
    delete (cmd);
  }
}
