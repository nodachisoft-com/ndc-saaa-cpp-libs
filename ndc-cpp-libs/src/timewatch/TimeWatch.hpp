/**
 * @brief
 *  アプリケーション内の汎用的な時間計測用の機能を提供する
 *  処理を実行している時間のみを計測する、アプリケーション内での経過時刻をスピード係数で
 *  調整可能な機能を提供する
 */
#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "ScheduleTask.hpp"
#include "../exception/index.hpp"

namespace nl
{

  class TimeWatch
  {
  private:
    /// @brief スケジュールタスクの一覧
    std::vector<ScheduleTask> tasklist;

    /// @brief アプリケーション時間に反映させるスピード（0.5f なら 0.5倍の速度として計算）
    float appSpeed;

    /// @brief 計測した時間の合計
    int64_t timeFromStartMs;

    bool stopFlag;

  public:
    /// @brief
    ///   コンストラクタ
    ///   計測は stop 状態で始まる。計測開始する前に start で状態変化を行う必要がある
    ///   リアル時間に対しての速度の係数は 1.0f （100%）がデフォルト設定。
    TimeWatch();

    /// @brief 計測開始状態に変更する
    void start();

    /// @brief 計測停止状態に変更する
    void stop();

    /// @brief Start（計測可能）な状態かを判定する
    /// @return Start（計測可能）であれば true、Stop（停止）であれば false を返す
    bool isStart();

    /// @brief
    ///  完了済みのタスクを削除する。
    ///  全体の進捗計測対象から除外され、過去の完了済みタスクは参照不可となる
    // void removeCompletedTasks();
    void removeWholeTasks();

    /// @brief
    ///  時間の変更に対する、アプリ計測時間の速度係数を指定する。標準は 1.0f が指定されている
    ///  例）0.5f を指定した時、1秒 を addRealTime で追加したら、 0.5 秒がカウントアップされる
    /// @param speed 計測時間の係数（速度）
    void setAppSpeed(const float speed);

    /// @brief アプリ計測の速度係数を取得する。デフォルト値は 1.0f
    /// @return 速度係数
    const float getAppSpeed();

    /// @brief リアル時間の経過（ms）を通知する
    /// @param deltaTimeMs 経過した差分の時間（ms）
    void addRealTime(const int64_t deltaTimeMs);

    /// @brief タスクを追加する。スケジュールタスクコードは 0 以上である検査をする
    /// @param task 計測のため末尾に追加するタスク
    void pushTask(ScheduleTask task);

    /// @brief 指定したタスクコードが完了しているかを確認する。
    /// @param taskcode タスクコード
    /// @return タスクが完了しているなら true 。それ以外は false。（存在しない場合も false）
    bool checkTaskCompleted(const int taskcode);

    /// @brief 現在計測中のタスクのタスクコードを返す。計測中のタスクがない場合、 -1 を返す
    /// @return 計測中のタスクコード。ないなら -1 を返す
    int getNowProgressingTaskcode();

    /// @brief 現在処理中タスクの進捗を計算して返す。タスクが無ければ -1.0f を返す
    /// @return 0.0～1.0f で進捗率を返す
    float getProcessingTaskProgress();

    /// @brief 全体タスクから実施完了したぶんの進捗を計算して返す。
    /// @return 0.0～1.0f で進捗率を返す。タスクが無ければ 0.0f を返す
    float getWholeTasksProgress();

    /// @brief 計測を開始（Start）してからの累積時間をミリ秒で返す
    /// @return 累積時間（ms）
    int64_t getTimeFromStartMs();

    /// @brief 現在の時刻を UnitTime 形式で MS 単位で返す
    /// @return 現在時刻(ms)
    static int64_t getNowMs();

    static std::string getTimezone();

    /// @brief 現在の時刻を ISO8601 形式（YYYYMMDDThh:mm:ss.sss+09:00 形式で返す）
    /// @return 時刻を ISO8601 形式の文字列
    static std::string getNowMsAsStrIso8601();

    /// @brief 現在の時刻を hh:mm:ss.SSS 形式の文字列で返す
    /// @return 現在時刻（hh:mm:ss.SSS）の文字列
    static std::string getNowMsAsStr_hhmmssSSS();
  };
};
