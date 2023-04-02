/**
 * @brief 画像のキャンバス情報を扱う
 */
#pragma once

#include <string>
#include <iostream>
#include "DebugFontData.hpp"
#include "ColorRGB.hpp"

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

    /// @brief 共有されるフォントデータ
    static ImageCanvas *fontCanvas;

    static int sharedCountForFontCanvas;

  public:
    ImageCanvas(const int width, const int height)
      : width(width), height(height)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
      initDefaultFontData();
    }

    ~ImageCanvas()
    {
      free(data);
      sharedCountForFontCanvas--;
      if (sharedCountForFontCanvas == 0 )
      {
        // ImageCanvas オブジェクトの利用者が 0 になったら static 内容を削除
        delete fontCanvas;
      }
    }

    void set(int x, int y, ColorRGB& color);

    ColorRGB get(int x, int y);

    void clear(ColorRGB& color);

    void writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color);

    void writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color);

    void initDefaultFontData();

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


