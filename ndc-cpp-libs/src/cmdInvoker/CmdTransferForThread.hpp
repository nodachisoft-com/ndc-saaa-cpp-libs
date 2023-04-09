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
  static std::vector<std::unique_ptr<CmdTransferForThread>> threadList;


  class CmdTransferForThread
  {
  private:

    /// @brief コマンドとして処理する対象のキュー
    std::deque<std::unique_ptr<CmdBase>> cmdQueue;

    /// @brief true ならスレッド内ループが自律的に終了する
    bool endThreadFlag;

    /// @brief
    ///   スレッドを特定するための UID.
    int uid;

    /// @bried
    ///   MainThread で動作するなら true でセットされる
    bool mainThread;

    /// @brief スレッド動作時に使用。MainThread の場合は使用しない
    std::thread th;

  public:


    CmdTransferForThread(int uid, bool isMainThread)
      : cmdQueue()
       ,endThreadFlag(false)
       ,uid(uid)
    {
      cmdQueue.clear();
      if (isMainThread)
      {
        // Main Thread 用のコンストラクタとして初期化処理する
        //std::cout << "UID[" << uid << "] is initialized as Main Thread." << std::endl;
      } else {
        // Thread として起動してループ動作を行う
        //std::cout << "UID[" << uid << "]cmdQueue cleared." << cmdQueue.size() << std::endl;
        th = std::thread(CmdTransferForThread::createThread, this);
      }
      mainThread = isMainThread;
    }

 
    std::unique_ptr<CmdBase> readNextCmd();

    /// <summary>
    /// ループ実行処理を行う
    /// </summary>
    void doMainLoop()
    {
      while ( true ) {
        // std::cout << "UID[" << uid << "] RUN. cmd Queue=" << cmdQueue.size() << std::endl;
        std::unique_ptr<CmdBase> cmd = readNextCmd(); // ここが マルチスレッド未対応？
        if (cmd)
        {
          // コマンドが存在する
          cmd->_EntryExec();
        }

        if (endThreadFlag)
        {
          // 終了フラグが立っていたため、処理を終了させる
          return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
    }
  public: // 外部からアクセスが許されるメソッド
    
    /// <summary>
    /// コマンドを CmdInvoker に追加する
    /// </summary>
    /// <param name="elem">追加するコマンド</param>
    void appendCmd(std::unique_ptr<CmdBase> elem);

    static void initThread()
    {
      threadList.clear();
      // std::cout << "CmdTransfer Thread Cleared." << std::endl;
    }


    static void createThread(CmdTransferForThread *a)
    {
      a->doMainLoop();
    }

    static void createAsNewThread(int uid)
    {
      threadList.push_back(std::make_unique<CmdTransferForThread>(uid, false));
    }

    static void addThreadListAsMainThread(std::unique_ptr<CmdTransferForThread> a)
    {
      threadList.push_back(std::move(a));
    }


    static void endThreads()
    {
      
      size_t threadCount = threadList.size();
      //std::cout << "Call endThreads. thread count=" << threadCount << std::endl;
      for (size_t i = 0; i < threadCount; i++) {
        if ( !threadList[i]->mainThread )
        {
          threadList[i]->endThread();
        }
      }
      //std::cout << "Wait threas join()" << std::endl;
      for (size_t i = 0; i < threadCount; i++) {
        if (!threadList[i]->mainThread)
        {
          threadList[i]->th.join();
        }
      }
    }

  private:
    void endThread()
    {
      endThreadFlag = true;
    }
  };
}