/**
 * @brief
 *  TimeWatch に渡すことで、複数のタスクの積み上げ、残り時間を管理可能とする
 *
 */
#pragma once
#include <stdint.h>

namespace nl
{
  /// @brief タスクにかかる時間、残り時間、終了済みかを管理する
  class ScheduleTask
  {
  private:
    /// @brief TimeWatch 内で検索・識別するためのコード
    int taskcode;

    /// @brief タスク処理に必要とする時間（ミリ秒）
    int64_t relTimeMs;

    /// @brief 経過時間を計測
    int64_t progressMs;

    /// @brief タスクが完了していれば true が返る
    bool _isCompleted;

  public:
    /// @brief コンストラクタ。初期化を実施
    /// @param taskcode タスクを識別可能とするコード
    /// @param relTimeMs タスク処理に必要とする時間（ミリ秒）
    ScheduleTask(const int taskcode, const int64_t relTimeMs);

    /// @brief ミリ秒を経過時間として加算する
    /// @param addDeltaTimeMs 追加する経過時間
    /// @return 予定した時間をオーバーした、超過分の ms を返す
    int64_t addProgressMs(int64_t addDeltaTimeMs);

    /// @brief タスク処理にかかる処理 ÷ 計測時間を計算して進捗率を返す
    /// @return 進捗を 0.0f ～ 1.0f
    float getTaskProgress();

    /// @brief タスクコードを返す
    int getTaskcode();

  public: // ===== Accessor Methods =======
    /// @brief タスク処理に必要とする時間（ミリ秒）を返す
    int64_t getRelTimeMs();

    /// @brief タスクで進行した時間をミリ秒で返す
    /// @return 進行済みの時間（ミリ秒） [0, relTimeMs] の間で結果を返す
    int64_t getProgressMs();

    /// @brief タスクが完了していれば true が返る
    bool isCompleted();
  };
}
