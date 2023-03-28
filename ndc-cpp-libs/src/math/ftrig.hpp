#pragma once
#include <cmath>
#include <unordered_map>
#include <vector>

namespace nl
{
  /// @brief 三角関数の近似値高速計算を行う cos / sin
  class Ftrig
  {
  private:
    /// @brief 高速計算のため事前にキャッシュ計算する精度（要素）.
    const int CACHE_SIZE = 64 * 1024;
    const float PI = 3.14159265358979323846f;
    float interval_size;

    /// @brief 事前に cos 計算結果をキャッシュをしておく
    std::vector<float> cos_table;

    /// @brief 事前に sin 計算結果をキャッシュをしておく
    std::vector<float> sin_table;

  public:
    Ftrig();
    float fsin(float x);
    float fcos(float x);
  };
}
