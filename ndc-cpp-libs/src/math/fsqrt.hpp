#pragma once
#include <cmath>
#include <unordered_map>
#include <vector>

namespace nl
{
  /// @brief
  ///  平方根の近似値を高速で求める。MAX_DIGITS 以上は標準ライブラリの sqrt で計算する
  ///  float が 4 バイトという前提で実行可能。
  ///  ただし、c++標準ライブラリの std::sqrt は十分高速であり、使用する意味はほぼない
  ///  fsqrt : sqrt の実行速度は 2:3 程度
  static constexpr float fastsqrt(const float &x)
  {
    float xHalf = 0.5f * x;
    int tmp = 0x5F3759DF - (*(int *)&x >> 1);
    float xRes = *(float *)&tmp;
    xRes *= (1.5f - (xHalf * xRes * xRes));
    return xRes * x;
  }
}
