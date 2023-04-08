#include <string>
//#include <memory>
//#include <chrono>
#include "../../pch.h"
//#include "../../../ndc-cpp-libs/src/cmdInvoker/CmdTransferThreadMgr.hpp"


using namespace nl;
using namespace std::literals::string_literals;

/*
std::vector<CmdTransferForThread> list;
std::vector<std::thread> threadList;

void _runThread()
{
  // 外部から Thread にアクセス可能とする
  CmdTransferForThread cmd;
  list.push_back(cmd);

  // Thread 処理を開始
  cmd.doMainLoop();
}
*/
class A;
std::vector<A> list;
std::vector<std::thread> thlist;

class A {
  public:
    bool finishFlag ;
    A() : finishFlag(false) {
      list.clear();
      thlist.clear();
    }
    void run()
    {
      while(true){
        std::cout << "RUN" << std::endl;
        if (finishFlag)
        {
          return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
    }

    static void createThread(A &a)
    {
      a.run();
    }

    void createNewThread()
    {
      A a;
      list.push_back(a);

      thlist.emplace_back(std::thread(&A::createThread, a));
      //std::thread th(createThread, a);
      // thlist.push_back(th);
    }

    static void endThreads()
    {
      size_t threadCount = list.size();
      for (size_t i = 0; i < threadCount; i++) {
        list[i].endThread();
      }
      for (size_t i = 0; i < threadCount; i++) {
        thlist[i].join();
      }
    }

  private:
    void endThread()
    {
      finishFlag = true;
    }
};





// Thread 用のコマンド転送用の送信・受信を直列実行する
TEST(CmdTransferThreadMgr, case1)
{
  // CmdTransferThreadMgr mgr;
  //mgr.runCmdTransferThraed();

  A a;


  std::this_thread::sleep_for(std::chrono::microseconds(1000));
  //mgr.finishAllThread();



  std::cout << "test" << std::endl;

}
