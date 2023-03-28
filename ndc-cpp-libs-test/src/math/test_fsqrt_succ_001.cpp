#include <cmath>
#include "../../pch.h"

using namespace nl;

// float sqrt 計算の誤差が想定通り小さいこと
/*
TEST(Fsqrt, fsqrt_case001)
{
  EXPECT_EQ(fastsqrt(-0.1f), 0.0f); // 負の数には 0.0f を返す

  float interval = 0.01f;       // sqrt 関数で検査する入力の間隔
  float acceptableDelta = 0.1f; // 許容誤差範囲( 0.1 % 以内)
  float maxDelta = 0.0f;
  float maxDeltaX = -1.0f; // 最大誤差が発生したインプットの値

  // 0.0f～10000.0f の間、0.01f 刻みで誤差が 0.00015f 以内であること
  for (int i = 0; i * interval < 10000.0f; i++)
  {
    float x = i * interval;
    float val = fastsqrt(x);
    float expComp = sqrtf(x);

    // 精密な計算と、高速計算結果の差が許容誤差範囲内であること
    float delta = abs(val - expComp) / x;
    if (delta > maxDelta)
    {
      // 最大誤差を更新
      maxDelta = delta;
      maxDeltaX = x;
    }
  }

  // 計算結果確認
  if (maxDelta > acceptableDelta)
  {
    // デバッグ情報出力
    std::string msg("Fsqrt#fsqrt_case001: ");
    msg += "Acceptable Delta Percent=" + std::to_string(acceptableDelta);
    msg += ", maxDelta Percent=" + std::to_string(maxDelta);
    msg += ", maxDeltaX=" + std::to_string(maxDeltaX);
    Logger log;
    log.errorLog(msg);

    EXPECT_TRUE(maxDelta < acceptableDelta);
  }
}
*/

/// @brief 速度計測し、標準ライブラリの sqrt と比較して少し高速であることを確認する
TEST(Fsqrt, fsqrt_vs_stdsqrt_case001)
{
  TimeWatch timer;

  int tryloopTo = 20;
  float tryCountTo = 100 * 1000;
  float interval = 0.0001f;
  int64_t time1begin = timer.getNowMs();
  double sum1 = 0.0;
  for (int j = 0; j < tryloopTo; j++)
  {
    for (float i = 0; i < tryCountTo; i += 1.0f)
    {
      sum1 += (double)nl::fastsqrt(i * interval);
    }
  }
  int64_t time1 = timer.getNowMs() - time1begin;

  int64_t time2begin = timer.getNowMs();
  double sum2 = 0.0;
  for (int j = 0; j < tryloopTo; j++)
  {
    for (float i = 0; i < tryCountTo; i += 1.0f)
    {
      sum2 += sqrtf(i * interval);
    }
  }
  int64_t time2 = timer.getNowMs() - time2begin;

  if (time2 < time1)
  {
    //  fastsqrt のほうが遅い場合
    std::string msg = std::string()
                          .append("fsqrt was slow! ")
                          .append("fsqrt=")
                          .append(std::to_string(time1))
                          .append("[ms], std::math.sqrt=")
                          .append(std::to_string(time2))
                          .append("[ms]");
    logger.error << msg << std::endl;
  }
}
