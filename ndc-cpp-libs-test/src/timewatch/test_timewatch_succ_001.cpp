#include "../../pch.h"

using namespace nl;

// 現在の時刻を取得する
TEST(TimeWatch, getNowMs_case001)
{
  int64_t nowUnixTimeMs = TimeWatch::getNowMs();
  // 想定されるような範囲に結果が含まれている。（2023/1/1～ 2123/1/1）
  EXPECT_TRUE((int64_t)1672498800 * 1000 < nowUnixTimeMs && nowUnixTimeMs < (int64_t)4828172400 * 1000L);
}

TEST(TimeWatch, getNowMsAsStrIso8601_case001)
{
  std::string time = TimeWatch::getNowMsAsStrIso8601();
  EXPECT_EQ(time.size(), 24);
}

TEST(TimeWatch, getNowMsAsStr_hhmmssSSS_case001)
{
  std::string time = TimeWatch::getNowMsAsStr_hhmmssSSS();
  EXPECT_EQ(time.size(), 12);
}

TEST(TimeWatch, getNowMsAsStr_initialized_case001)
{
  TimeWatch watch;

  // 初期化直後の状態を確認
  EXPECT_EQ(watch.isStart(), false);                   // Watch は停止している
  EXPECT_EQ(watch.getAppSpeed(), 1.0f);                // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 0);            // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), -1);    // 処理中のタスクコードを応答。存在しないので -1
  EXPECT_EQ(watch.getProcessingTaskProgress(), -1.0f); // 処理中のタスクコードを応答。存在しないので -1.0f
  EXPECT_EQ(watch.getWholeTasksProgress(), 0.0f);      // 全体の進捗は 0 %
  EXPECT_EQ(watch.getTimeFromStartMs(), 0);            // 全体の進捗は 0 ms

  // タスクを追加する
  ScheduleTask task1(1, 10); // タスクコード 1、処理時間 10 ms
  ScheduleTask task2(2, 20); // タスクコード 2、処理時間 20 ms
  ScheduleTask task3(3, 30); // タスクコード 3、処理時間 30 ms
  ScheduleTask task4(4, 40); // タスクコード 4、処理時間 40 ms
  watch.pushTask(task1);
  watch.pushTask(task2);
  watch.pushTask(task3);
  watch.pushTask(task4);

  EXPECT_EQ(watch.isStart(), false);                  // Watch は停止している
  EXPECT_EQ(watch.getAppSpeed(), 1.0f);               // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 0);           // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), 1);    // 処理中のタスクコードを応答。最初に追加したタスク 1 が返る
  EXPECT_EQ(watch.getProcessingTaskProgress(), 0.0f); // 処理中タスクの進捗を応答。 0.0f が返る
  EXPECT_EQ(watch.getWholeTasksProgress(), 0.0f);     // 全体の進捗は 0 %

  // 時間を経過させる（5ms）が、Watch は stop 状態なので加算時間は 0 ms である
  watch.addRealTime(5);
  EXPECT_EQ(watch.isStart(), false);                  // Watch は停止している
  EXPECT_EQ(watch.getAppSpeed(), 1.0f);               // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 0);           // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), 1);    // 処理中のタスクコードを応答。最初に追加したタスク 1 が返る
  EXPECT_EQ(watch.getProcessingTaskProgress(), 0.0f); // 処理中タスクの進捗を応答。 0.0f が返る
  EXPECT_EQ(watch.getWholeTasksProgress(), 0.0f);     // 全体の進捗は 0 %

  // 計測を開始状態に変更
  watch.start();
  EXPECT_EQ(watch.isStart(), true); // Watch は開始している

  // 時間を経過させる（5ms）
  watch.addRealTime(5);
  EXPECT_EQ(watch.isStart(), true);                   // Watch は開始している
  EXPECT_EQ(watch.getAppSpeed(), 1.0f);               // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 5);           // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), 1);    // 処理中のタスクコードを応答。最初に追加したタスク 1 が返る
  EXPECT_EQ(watch.getProcessingTaskProgress(), 0.5f); // 処理中タスクの進捗を応答。 0.5f が返る
  EXPECT_EQ(watch.getWholeTasksProgress(), 0.05f);    // 全体の進捗は 5ms / 100ms = 5 %

  // アプリ内時間経過の速度を 0.5 倍に変更する
  watch.setAppSpeed(0.5f);

  // 時間を経過させ（realTime = 80ms、アプリ内で 40ms）タスクを 2 つ完了状態にする
  watch.addRealTime(40 * 2);
  EXPECT_EQ(watch.isStart(), true);                   // Watch は開始している
  EXPECT_EQ(watch.getAppSpeed(), 0.5f);               // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 45);          // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), 3);    // 処理中のタスクコードを応答。タスク 3 が返る
  EXPECT_EQ(watch.getProcessingTaskProgress(), 0.5f); // 処理中タスクの進捗を応答。15ms/30ms = 0.5f が返る
  EXPECT_EQ(watch.getWholeTasksProgress(), 0.45f);    // 全体の進捗は 45ms / 100ms = 45 %

  // タスクが完了していることを確認する
  EXPECT_EQ(watch.checkTaskCompleted(1), true);  // タスク 1 は完了状態
  EXPECT_EQ(watch.checkTaskCompleted(2), true);  // タスク 2 は完了状態
  EXPECT_EQ(watch.checkTaskCompleted(3), false); // タスク 3 は未完了状態
  EXPECT_EQ(watch.checkTaskCompleted(4), false); // タスク 4 は未完了状態

  // 時間を経過させ、すべてのタスクを完了状態とする
  watch.addRealTime(1000 * 2);
  EXPECT_EQ(watch.isStart(), true);                    // Watch は開始している
  EXPECT_EQ(watch.getAppSpeed(), 0.5f);                // アプリの速度
  EXPECT_EQ(watch.getTimeFromStartMs(), 1045);         // 開始からの累積時間
  EXPECT_EQ(watch.getNowProgressingTaskcode(), -1);    // 処理中のタスクコードを応答。全タスク完了で -1 が返る
  EXPECT_EQ(watch.getProcessingTaskProgress(), -1.0f); // 処理中タスクの進捗を応答。全タスク完了で -1 が返る
  EXPECT_EQ(watch.getWholeTasksProgress(), 1.0f);      // 全体の進捗は 100ms / 100ms = 100 %
}
