/**
 * @brief 画像のキャンバス情報を扱う
 */
#pragma once

#include <string>
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
      : width(width), height(height), data(nullptr)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
    }

    ~ImageCanvas()
    {
      freeResources();
    }

    /// <summary>
    /// リソースを初期化する
    /// </summary>
    void freeResources()
    {
      free(data);
    }
  };
};


