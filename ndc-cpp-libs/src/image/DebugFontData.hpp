#pragma once
#include <string>
#include "ImageCanvas.hpp"

namespace nl
{
  class DebugFontData
  {
  private: // 非公開メンバ
    std::string FONT_DATA;

    // GreyScale から復元されたキャンバス画像データ
    ImageCanvas fontCanvas;

  private: // 非公開Method
    void loadToCanvas();


  public:
    DebugFontData();
    ~DebugFontData();
    static const int width = 1045;
    static const int height = 21;
    unsigned char getGreyScale256(const int x, const int y);

    ImageCanvas* getFontCanvas()
    {
      return &fontCanvas;
    }
  };

  // グローバルで一つの DebugFontData を保持する
  extern DebugFontData DEBUG_FONT_DATA;
}
