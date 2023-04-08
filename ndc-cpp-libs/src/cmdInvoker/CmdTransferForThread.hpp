#pragma once
#include <deque>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include "cmd/CmdBase.hpp"

namespace nl
{
  class CmdTransferForThread;

  /// @brief CmdInvoker を動作させるスレッド一覧を管理
  // static std::vector<CmdTransferForThread *> threadList;
  static std::vector<std::unique_ptr<CmdTransferForThread>> threadList;


  class CmdTransferForThread
  {
  private:

    /// @brief コマンドとして処理する対象のキュー
    std::deque<std::unique_ptr<CmdBase>> cmdQueue;

    /// @brief true ならスレッド内ループが自律的に終了する
    bool endThreadFlag;


    int uid;

  public:
    std::thread th;

    CmdTransferForThread(int uid)
      : cmdQueue()
       ,endThreadFlag(false)
       ,uid(uid)
       // , th(&CmdTransferForThread::createThread, *this)
    {
      cmdQueue.clear();
      std::cout << "UID[" <<uid << "]cmdQueue cleared." << cmdQueue.size() << std::endl;

      th = std::thread(CmdTransferForThread::createThread, this);
    }
       /*
    ~CmdTransferForThread() {
      std::cout << "CmdTransferForThread Destruction" << std::endl;
    }*/
    std::unique_ptr<CmdBase> readNextCmd();

    /// <summary>
    /// ループ実行処理を行う
    /// </summary>
    void doMainLoop()
    {
      int i = 1000;
      while ( i-- > 0 ) {
        std::cout << "UID[" << uid << "] RUN. cmd Queue=" << cmdQueue.size() << std::endl;
        
        /*
        std::unique_ptr<CmdBase> cmd = readNextCmd(); // ここが マルチスレッド未対応？
        if (!cmd)
        {
          // コマンドがなくなったので終了

        }
        cmd->_EntryExec();
        */



        if (endThreadFlag)
        {
          // 終了フラグが立っていたため、処理を終了させる
          std::cout << "UID[" << uid << "]FINISH!" << std::endl;
          return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1000 * 10));
      }
    }
  public: // 外部からアクセスが許されるメソッド
    
    /// <summary>
    /// コマンドを CmdInvoker に追加する
    /// </summary>
    /// <param name="elem">追加するコマンド</param>
    void appendCmd(std::unique_ptr<CmdBase> elem);

    //void finishRunning()
    //{
    //  endThreadFlag = true;
    //}
    static void initThread()
    {
      threadList.clear();
      std::cout << "CmdTransfer Thread Cleared." << std::endl;
    }


    static void createThread(CmdTransferForThread *a)
    {
      
      a->doMainLoop();
    }

    static void createNewThread(int uid)
    {
      threadList.push_back(std::make_unique<CmdTransferForThread>(uid));
    }

    static void endThreads()
    {
      std::cout << "Call endThreads." << std::endl;
      size_t threadCount = threadList.size();
      for (size_t i = 0; i < threadCount; i++) {
        threadList[i]->endThread();
      }
      std::cout << "Wait threas join()" << std::endl;
      for (size_t i = 0; i < threadCount; i++) {
        threadList[i]->th.join();
      }
      //for (size_t i = 0; i < threadCount; i++) {
      //  delete threadList[i];
      //}
    }

  private:
    void endThread()
    {
      endThreadFlag = true;
    }
  };
}