#include "../../pch.h"

using namespace nl;

// タスク開始～終了までの正常系の流れを確認
TEST(TimeWatch, constructor_case001)
{
  ScheduleTask task(999, 400); // タスクコード 999、タスク時間 400 ms

  // 初期化直後
  EXPECT_EQ(task.getTaskProgress(), 0.0f); // 進行が 0 % であること
  EXPECT_EQ(task.getTaskcode(), 999);      // タスクコードは 999
  EXPECT_EQ(task.getRelTimeMs(), 400);     // タスク時間は 400
  EXPECT_EQ(task.getProgressMs(), 0);      // 進捗時間は 0 ms
  EXPECT_EQ(task.isCompleted(), false);    // 進捗の完了は false

  // 10% 進行させる
  task.addProgressMs(40);
  EXPECT_EQ(task.getTaskProgress(), 0.10f); // 進行が 10 % であること
  EXPECT_EQ(task.getTaskcode(), 999);       // タスクコードは 999
  EXPECT_EQ(task.getRelTimeMs(), 400);      // タスク時間は 400
  EXPECT_EQ(task.getProgressMs(), 40);      // 進捗時間は 40 ms
  EXPECT_EQ(task.isCompleted(), false);     // 進捗の完了は false

  // さらに 20% 進行させる(30%となる)
  task.addProgressMs(80);
  EXPECT_EQ(task.getTaskProgress(), 0.30f); // 進行が 30 % であること
  EXPECT_EQ(task.getTaskcode(), 999);       // タスクコードは 999
  EXPECT_EQ(task.getRelTimeMs(), 400);      // タスク時間は 400
  EXPECT_EQ(task.getProgressMs(), 120);     // 進捗時間は 120 ms
  EXPECT_EQ(task.isCompleted(), false);     // 進捗の完了は false

  // 100 % 進行させる
  int64_t left = task.addProgressMs(400);
  EXPECT_EQ(left, 120);                     // タスクを完了後、120 ms の時間が余っている
  EXPECT_EQ(task.getTaskProgress(), 1.00f); // 進行が 100 % であること(100% を超過していないこと)
  EXPECT_EQ(task.getTaskcode(), 999);       // タスクコードは 999
  EXPECT_EQ(task.getRelTimeMs(), 400);      // タスク時間は 400
  EXPECT_EQ(task.getProgressMs(), 400);     // 進捗時間は 400 ms (400 を超過していないこと）
  EXPECT_EQ(task.isCompleted(), true);      // 進捗の完了は true

  // 10 % 追加で進行させる（変化がないこと）
  left = task.addProgressMs(40);
  EXPECT_EQ(left, 40);                      // タスクを完了後、40 ms の時間が余っている
  EXPECT_EQ(task.getTaskProgress(), 1.00f); // 進行が 100 % であること(100% を超過していないこと)
  EXPECT_EQ(task.getTaskcode(), 999);       // タスクコードは 999
  EXPECT_EQ(task.getRelTimeMs(), 400);      // タスク時間は 400
  EXPECT_EQ(task.getProgressMs(), 400);     // 進捗時間は 400 ms (400 を超過していないこと）
  EXPECT_EQ(task.isCompleted(), true);      // 進捗の完了は true
}
