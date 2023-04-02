#include "ImageCanvas.hpp"

using namespace nl;




void ImageCanvas::set(int x, int y, ColorRGB& color)
{
  //int pos = width * y + x;
  //data[pos].b = color.b;
  //data[pos].g = color.g;
  //data[pos].r = color.r;
  data.setWithIgnoreOutOfRangeData(x,y,color);
}


ColorRGB ImageCanvas::get(int x, int y)
{
  //int pos = width * y + x;
  //ColorRGB result;
  //result.b = data[pos].b;
  //result.g = data[pos].g;
  //result.r = data[pos].r;
  //return result;
  return data.getWithIgnoreOutOfRangeData(x,y);
}

void ImageCanvas::clear(ColorRGB& color)
{
  //unsigned char r = color.r;
  //unsigned char g = color.g;
  //unsigned char b = color.b;
  //int size = width * height;
  //for (int i = 0; i < size; i++)
  //{
  //  data[i].b = b;
  //  data[i].g = g;
  //  data[i].r = r;
  //}
  data.setWholeData(color);
}

// フォントデータ読み込み
void ImageCanvas::writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color)
{
  int size = (int)text.size();
  for (int index = 0; index < size; index++)
  {
    const char drawCh = text[index];
    writeChar(destBeginX + index * DebugFontData::FONTWIDTH, destBeginY, drawCh, color);
  }
}

void ImageCanvas::writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color)
{
  // 描画対象の範囲外の文字コードが渡された
  if (ch < ' ' || '~' < ch)
  {
    // 何も描画しない
    return;
  }
  int width = getWidth();
  int height = getHeight();


  // コピーするフォント画像データの開始 x 座標を求める
  int fontImgBeginX = (ch - ' ') * DebugFontData::FONTWIDTH;

  for (int y = 0; y < DebugFontData::FONTHEIGHT; y++)
  {
    for (int x = 0; x < DebugFontData::FONTWIDTH; x++)
    {
      int destX = destBeginX + x;
      int destY = destBeginY + y;
      if ((destX < 0) || (destX >= width) ||
        (destY < 0) || (destY >= height))
      {
        // 描画先がデータ範囲外
        continue;
      }

      // フォント画像を書き込み。白色は透明化
      // フォント画像が黒色に近い程、フォント色側に書き込み先の元色を近づけることで、印字を表現。
      ColorRGB fontBaseColor = fontCanvas->get(fontImgBeginX + x, y);
      ColorRGB beforeColor = get(destX, destY);

      // 0.0f は字に近い。1.0f は字から遠い
      float distance = 1.0f - (fontBaseColor.r / 255.0f);
      ColorRGB updateColor;
      updateColor.r = (unsigned char)(color.r * distance + beforeColor.r * (1.0f - distance));
      updateColor.g = (unsigned char)(color.g * distance + beforeColor.g * (1.0f - distance));
      updateColor.b = (unsigned char)(color.b * distance + beforeColor.b * (1.0f - distance));

      set(destX, destY, updateColor);
    }
  }
}

ImageCanvas* ImageCanvas::fontCanvas = nullptr;
int ImageCanvas::sharedCountForFontCanvas = 0;

void ImageCanvas::initDefaultFontData()
{
  if (sharedCountForFontCanvas > 0) {
    // fontCanvas の二回目以降の初期化ループが発生しないように制御
    return;
  }
  sharedCountForFontCanvas++;

  DebugFontData font;
  fontCanvas = new ImageCanvas(DebugFontData::width, DebugFontData::height);

  int fontCanvasWidth = fontCanvas->getWidth();
  int fontCanvasHeight = fontCanvas->getHeight();
  std::cout << "fontCanvas w=" << fontCanvas->getWidth() << " , h="
    << fontCanvas->getHeight() << std::endl;


  // フォントデータ読み込み
  for (int v = 0; v < fontCanvasHeight; v++)
  {
    for (int u = 0; u < fontCanvasWidth; u++)
    {
      ColorRGB color;
      unsigned char grey = font.getGreyScale256(u, v);
      grey = (grey >= 250) ? 255 : grey; // 253 以上は 255 に定義
      color.r = grey;
      color.g = grey;
      color.b = grey;
      fontCanvas->set(u, v, color);

    }
  }
} 