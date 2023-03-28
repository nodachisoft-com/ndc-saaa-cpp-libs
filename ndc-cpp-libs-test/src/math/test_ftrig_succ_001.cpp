#include <cmath>
#include "../../pch.h"

using namespace nl;

// float sin 計算の誤差が想定通り小さいこと
/*
TEST(Ftrig, fsin_case001)
{
  Ftrig math;
  float interval = 0.01f;          // sin 関数で検査する入力の間隔
  float acceptableDelta = 0.0001f; // 許容誤差範囲
  float maxDelta = 0.0f;
  for (int i = 0; i < 10000; i++)
  {
    float rad = i * interval;
    float val = math.fsin(rad);
    float expComp = sinf(rad);

    // 精密な計算と、高速計算結果の差が許容誤差範囲内であること
    float delta = abs(val - expComp);
    maxDelta = delta > maxDelta ? delta : maxDelta;
  }

  // 計算結果確認
  EXPECT_TRUE(maxDelta < acceptableDelta);
}

// float cos 計算の誤差が想定通り小さいこと
TEST(Ftrig, fcos_case001)
{
  Ftrig math;
  float interval = 0.01f;          // cos 関数で検査する入力の間隔
  float acceptableDelta = 0.0001f; // 許容誤差範囲
  float maxDelta = 0.0f;
  for (int i = 0; i < 10000; i++)
  {
    float rad = i * interval;
    float val = math.fcos(rad);
    float expComp = cosf(rad);

    // 精密な計算と、高速計算結果の差が許容誤差範囲内であること
    float delta = abs(val - expComp);
    maxDelta = delta > maxDelta ? delta : maxDelta;
  }

  // 計算結果確認
  EXPECT_TRUE(maxDelta < acceptableDelta);
}

TEST(Ftrig, fcos_time_case001)
{
  TimeWatch timer;

  Ftrig math;
  float interval = 0.01f;          // cos 関数で検査する入力の間隔
  float acceptableDelta = 0.0001f; // 許容誤差範囲
  float maxDelta = 0.0f;
  long loopCount = 10000 * 100;
  int64_t time1begin = timer.getNowMs();
  for (long i = 0; i < loopCount; i++)
  {
    float val = math.fcos(i * interval);
  }
  int64_t time1 = timer.getNowMs() - time1begin;

  int64_t time2begin = timer.getNowMs();
  for (long i = 0; i < loopCount; i++)
  {
    float expComp = cosf(i * interval);
  }
  int64_t time2 = timer.getNowMs() - time2begin;

  // 計算結果確認
  if (time1 > time2)
  {
    // fastcos のほうが遅い場合

    std::string message("Ftrig#fcos_time_case001 : fastcos=");
    message.append(std::to_string(time1));
    message.append("[ms], std::math.cos=");
    message.append(std::to_string(time2));
    message.append("[ms]");
    logger.error << message << std::endl;
  }
}
*/