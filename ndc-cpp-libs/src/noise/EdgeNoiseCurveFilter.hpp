/**
 * @brief
 *  2D でフチが 0.0f、中央部を 1.0f に近づける機能を提供する
 */
#pragma once
#include <iostream>

namespace nl
{

  class EdgeNoiseCurveFilter
  {
  private:
    bool isInit;

    /// @brief 縁からの幅を定義 [0.0f,0.5f]
    float edgeWidePerc;

    /// @brief 与えられた a と b を比較し最小値を返す
    /// @param a
    /// @param b
    /// @return
    float min(const float a, const float b)
    {
      return (a <= b) ? a : b;
    }

    float plerp(const float x)
    {
      return x * x * (3.0f - 2.0f * x);
    }

  public:
    /// @brief デフォルトコンストラクタ定義
    EdgeNoiseCurveFilter(void);

    /// @brief 初期化する。
    /// @param edgeWidePerc 縁 0.0f - 中央 1.0f の距離を全体サイズからの % で指定。幅は [0.0f,0.5f]
    EdgeNoiseCurveFilter(const float edgeWidePerc);

    /// @brief 初期化する。
    /// @param edgeWidePerc 縁 0.0f - 中央 1.0f の距離を全体サイズからの % で指定。幅は [0.0f,0.5f]
    /// @return 正常に初期化完了で true。初期化済みだったり異常終了した場合は false
    bool init(const float edgeWidePerc);

    /// @brief 指定の位置の高さを取得する
    /// @param[in] x 位置の指定。 [0.0-1.0]
    /// @param[in] y 位置の指定。 [0.0-1.0]
    /// @return 0.0f-1.0f の間で高さを取得
    float pos2(const float x, const float y);
  };
};
