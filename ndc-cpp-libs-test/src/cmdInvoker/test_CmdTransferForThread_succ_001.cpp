#include <string>
#include <memory>
#include "../../pch.h"


using namespace nl;
using namespace std::literals::string_literals;

// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferForThread, append_readNext_case1)
{
  // From MainUI
  CmdTransferForThread cmdTransfer(0,true);
  cmdTransfer.appendCmd(std::make_unique<ExitForceCmd>());


  int cmdExeCount = 0;

  // Brain UI cmdInvoker シミュレータ
  while (true)
  {
    std::unique_ptr<CmdBase> cmd = cmdTransfer.readNextCmd();
    if (!cmd)
    {
      // コマンドがなくなったので終了
      break;
    }
    cmdExeCount++;
  }


  //CmdTransferForThread::endThreads();
  std::cout << "test" << std::endl;

  // 1件のコマンドが実行された
  EXPECT_EQ(cmdExeCount, 1);
}
