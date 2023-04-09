#include <string>
#include <memory>
#include <chrono>
#include "../../pch.h"
//#include "../../../ndc-cpp-libs/src/cmdInvoker/CmdTransferThreadMgr.hpp"


using namespace nl;
using namespace std::literals::string_literals;



// Thread 用のコマンド転送用の送信・受信を直列実行する
// 10個のCommandInvoker を動作させる Thread プログラムを実行し
// 作成したのち、すべての CmdInvokerForThread を終了させる
TEST(CmdTransferThreadMgr, case1)
{

  CmdTransferForThread::initThread();
  for ( int i = 0 ; i < 10 ; i++){
    CmdTransferForThread::createAsNewThread(i);
  }
  CmdTransferForThread::endThreads();
}

