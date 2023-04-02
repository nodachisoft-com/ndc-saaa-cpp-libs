/**
 * @brief RGB形式の画素情報を扱う
 */
#pragma once

namespace nl
{

  /// @brief 1ピクセルあたりの RGB 形式の画素情報
  class ColorRGB
  {
    public:
    /// @brief 赤色要素
    unsigned char r;

    /// @brief 緑色要素
    unsigned char g;

    /// @brief 青色要素
    unsigned char b;

    ColorRGB()
      : r(0), g(0), b(0)
    {}

    ColorRGB(unsigned char r, unsigned char g, unsigned char b)
      : r(r), g(g), b(b)
    {
    }
  };
}