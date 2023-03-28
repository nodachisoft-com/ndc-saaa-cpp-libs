/**
 * @brief 2次元空間上での Vector を扱う機能を提供する
 */

#pragma once
namespace nl
{
  class Vector2d
  {
  public:
    /// @brief ベクトルの x 成分
    float x;
    /// @brief ベクトルの y 成分
    float y;

  public:
    /// @brief ベクトル成分を (0.0f, 0.0f) で初期化する
    Vector2d();

    /// @brief ベクトル成分を (x , y) で初期化する
    /// @param x 初期化する x 成分
    /// @param y 初期化する y 成分
    Vector2d(const float x, const float y);

    /// @brief 長さを 1 に正規化した Unit Vector を計算する。
    Vector2d normalize() const;

    /// @brief ベクトルの長さを計算する。
    /// @return ベクトルの長さ
    float magnitude() const;

    Vector2d operator+(const Vector2d &other) const
    {
      return Vector2d(x + other.x, y + other.y);
    }

    Vector2d operator-(const Vector2d &other) const
    {
      return Vector2d(x - other.x, y - other.y);
    }

    Vector2d operator*(float scalar) const
    {
      return Vector2d(x * scalar, y * scalar);
    }

    Vector2d operator*(int scalar) const
    {
      return Vector2d(x * scalar, y * scalar);
    }

    Vector2d operator/(float scalar) const
    {
      return Vector2d(x / scalar, y / scalar);
    }

    Vector2d operator/(int scalar) const
    {
      return Vector2d(x / (float)scalar, y / (float)scalar);
    }
  };
}
