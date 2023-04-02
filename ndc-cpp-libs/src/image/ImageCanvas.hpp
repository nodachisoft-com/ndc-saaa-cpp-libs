/**
 * @brief 画像のキャンバス情報を扱う
 */
#pragma once

#include <string>
#include <iostream>
//#include "BitmapImage.hpp"
#include "ColorRGB.hpp"
//#include "DebugFontData.hpp"


namespace nl
{

  /// @brief 2次元の画像キャンバス情報をもつクラス
  class ImageCanvas
  {
  public:
    /// @brief 高さ
    int height;

    /// @brief 幅
    int width;

    /// @brief データ本体
    ColorRGB* data;

  public:
    ImageCanvas(const int width, const int height)
      : width(width), height(height)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
    }

    ~ImageCanvas()
    {
      free(data);
    }


    void set(int x, int y, ColorRGB& color);


    ColorRGB get(int x, int y);

    void clear(ColorRGB& color);


  public: // アクセサ
    int getWidth()
    {
      return width;
    }

    int getHeight()
    {
      return height;
    }
  };
};


