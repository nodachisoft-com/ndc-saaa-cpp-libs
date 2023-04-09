#include <string>
#include <memory>
#include <chrono>
#include "../../pch.h"
//#include "../../../ndc-cpp-libs/src/cmdInvoker/CmdTransferThreadMgr.hpp"


using namespace nl;
using namespace std::literals::string_literals;



// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferThreadMgr, case1)
{
  // OK
  std::cout << "START" << std::endl;
  CmdTransferForThread::initThread();
  for ( int i = 0 ; i < 10 ; i++){
    CmdTransferForThread::createAsNewThread(i);
  }
  CmdTransferForThread::endThreads();

  // FAIL();
}

