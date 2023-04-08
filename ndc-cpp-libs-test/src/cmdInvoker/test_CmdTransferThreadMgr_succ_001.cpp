#include <string>
#include <memory>
#include <chrono>
#include "../../pch.h"
//#include "../../../ndc-cpp-libs/src/cmdInvoker/CmdTransferThreadMgr.hpp"


using namespace nl;
using namespace std::literals::string_literals;


/*
class A;

//std::vector<std::thread> thlist;
static std::vector<A> list;

class A {
  private:

    std::thread th;

  public:

    bool finishFlag ;
    A() : finishFlag(false)
      , th(&A::createThread, this)
    {

    }
    void run()
    {
      while(true){
        std::cout << "RUN" << std::endl;
        if (finishFlag)
        {
          std::cout << "FINISH!" << std::endl;
          return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        // return;
      }
    }

    static void createThread(A *a)
    {
      a->run();
    }

    static void createNewThread()
    {
      
     list.push_back(A());
    }

    static void endThreads()
    {
      size_t threadCount = list.size();
      for (size_t i = 0; i < threadCount; i++) {
        list[i].endThread();
      }
      for (size_t i = 0; i < threadCount; i++) {
        list[i].th.join();
      }
    }

  private:
    void endThread()
    {
      finishFlag = true;
    }
};
*/



// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferThreadMgr, case1)
{
  std::cout << "START" << std::endl;
  CmdTransferForThread::initThread();
  for ( int i = 0 ; i < 10 ; i++){
    CmdTransferForThread::createNewThread(i);
  }
  CmdTransferForThread::endThreads();

  FAIL();
}

