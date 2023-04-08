#pragma once
#include <deque>
#include <thread>
#include <chrono>
#include "cmd/CmdBase.hpp"

namespace nl
{
  class CmdTransferForThread
  {
    /// @brief コマンドとして処理する対象のキュー
    std::deque<std::unique_ptr<CmdBase>> cmdQueue;

    /// @brief true ならスレッド内ループが自律的に終了する
    bool endThreadFlag;

  public:
    CmdTransferForThread()
      : cmdQueue()
       ,endThreadFlag(false) {}

    std::unique_ptr<CmdBase> readNextCmd();

    /// <summary>
    /// ループ実行処理を行う
    /// </summary>
    void doMainLoop()
    {
      while ( true ) {
        std::unique_ptr<CmdBase> cmd = readNextCmd();
        if (cmd == nullptr)
        {
          // コマンドがなくなったので終了

        }
        cmd->_EntryExec();
        std::this_thread::sleep_for(std::chrono::microseconds(1));

        if (endThreadFlag)
        {
          // 終了フラグが立っていたため、処理を終了させる
          return;
        }
      }
    }
  public: // 外部からアクセスが許されるメソッド
    
    /// <summary>
    /// コマンドを CmdInvoker に追加する
    /// </summary>
    /// <param name="elem">追加するコマンド</param>
    void appendCmd(std::unique_ptr<CmdBase> elem);

    void finishRunning()
    {
      endThreadFlag = true;
    }

  };
}