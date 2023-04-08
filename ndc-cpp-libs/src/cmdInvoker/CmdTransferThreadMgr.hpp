#pragma once
#include <vector>
#include <thread>
#include "CmdTransferForThread.hpp"

namespace nl
{
  class CmdTransferThreadMgr
  {
    /*
    private:
      static std::vector<CmdTransferForThread> list;
      static std::vector<std::thread> threadList;
    public:
      CmdTransferThreadMgr(){}


      /// thread 処理の開始
      static void _runThread()
      {
        // 外部から Thread にアクセス可能とする
        CmdTransferForThread cmd;
        list.push_back(cmd);

        // Thread 処理を開始
        cmd.doMainLoop();
      }

      void runCmdTransferThraed()
      {
        std::thread th = std::thread(_runThread);
        threadList.push_back(th);
      }

      void finishAllThread()
      {
        size_t t = list.size();
        for (size_t i = 0; i < t; i++)
        {
          CmdTransferForThread targetCmdTransfer= list.at(i);
          targetCmdTransfer.finishRunning();
        }

        for (size_t i = 0; i < t; i++)
        {
          threadList[i].join();
        }
      }

    */
  };
}