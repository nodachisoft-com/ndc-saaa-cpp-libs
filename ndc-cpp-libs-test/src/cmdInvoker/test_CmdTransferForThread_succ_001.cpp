﻿#include <string>
#include <memory>
#include "../../pch.h"


using namespace nl;
using namespace std::literals::string_literals;

// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferForThread, append_readNext_case1)
{
  // From MainUI
  CmdTransferForThread cmdTransfer;
  cmdTransfer.appendCmd(std::make_unique<ExitForceCmd>());

  // Brain UI cmdInvoker シミュレータ
  while (true)
  {
    std::unique_ptr<CmdBase> cmd = cmdTransfer.readNextCmd();
    if (cmd == nullptr)
    {
      // コマンドがなくなったので終了
      break;
    }
    cmd->_EntryExec();
  }
}
