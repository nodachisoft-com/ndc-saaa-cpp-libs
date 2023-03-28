/**
 * @brief
 *  四角形を扱う機能を提供する
 *  ２次元のデカルト座標上で、四角形を構成する４頂点は、 x,y 座標軸に対して 0度か 90度である。
 *  (30度回転などは表現できない)
 *  左上を 原点 (0,0) とし、コンピュータの画面で FullHD は右下は (1920, 1080) のように扱う
 */
#pragma once
#include "Vector2d.hpp"
#include "Point2d.hpp"

namespace nl
{
  class Rectangle2d
  {
  private:
    /// @brief 左 x 座標
    float Lx;

    /// @brief 右 x 座標
    float Rx;

    /// @brief 上 y 座標
    float Uy;

    /// @brief 下 y 座標
    float By;

  public: // ======= メソッド定義 ==========
    Rectangle2d();
    Rectangle2d(const Point2d LeftUpperPoint, const Point2d RightButtomPoint);

    /// @brief 四角形の左上の点
    Point2d getLeftUpperPoint();

    /// @brief 四角形の右下の点
    Point2d getRightBottomPoint();

    /// @brief  四角形の幅の長さを計算して返す
    /// @return 四角形の幅の長さ
    float getWidth();

    /// @brief 四角形の縦の長さを返す
    /// @return 四角形の縦の長さ
    float getHeight();

  private:
    /// @brief 与えられた 2 点から、四角形の左上、右下を計算してセットする
    /// @param p1 与えられた点 1
    /// @param p2 与えられた点 2
    void set2PointAsRectangle(const Point2d p1, const Point2d p2);

  public: // ===== アクセサメソッド =====
    float getLeftX() const { return Lx; }
    float getRightX() const { return Rx; }
    float getUpperY() const { return Uy; }
    float getBottomY() const { return By; }
  };
}
