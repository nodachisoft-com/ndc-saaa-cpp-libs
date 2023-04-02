#include "ImageCanvas.hpp"

using namespace nl;




void ImageCanvas::set(int x, int y, ColorRGB& color)
{
  int pos = width * y + x;
  data[pos].b = color.b;
  data[pos].g = color.g;
  data[pos].r = color.r;
}


ColorRGB ImageCanvas::get(int x, int y)
{
  int pos = width * y + x;
  ColorRGB result;
  result.b = data[pos].b;
  result.g = data[pos].g;
  result.r = data[pos].r;
  return result;
}

void ImageCanvas::clear(ColorRGB& color)
{
  unsigned char r = color.r;
  unsigned char g = color.g;
  unsigned char b = color.b;
  int size = width * height;
  for (int i = 0; i < size; i++)
  {
    data[i].b = b;
    data[i].g = g;
    data[i].r = r;
  }
}

// フォントデータ読み込み
/*
bool ImageCanvas::initializeFontdata()
{
  if (fontImage == nullptr) 
  {
    // 既に DebugFontData を読み込み済み
    return false;
  }
  DebugFontData font;
  fontImage = new BitmapImage(font.width, font.height);
  for (int v = 0; v < font.height; v++)
  {
    for (int u = 0; u < font.width; u++)
    {
      ColorRGB color;
      unsigned char grey = font.getGreyScale256(u, v);
      grey = (grey >= 250) ? 255 : grey; // 253 以上は 255 に定義
      color.r = grey;
      color.g = grey;
      color.b = grey;
      fontImage->set(u, v, color);
    }
  }
  return true;
}
*/

/*
void ImageCanvas::writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color)
{
  size_t size = text.size();
  for (size_t index = 0; index < size; index++)
  {
    const char drawCh = text[index];
    writeChar(destBeginX + index * fontWidth, destBeginY, drawCh, color);
  }
}

void ImageCanvas::writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color)
{
  if (fontImage == NULL)
  {
    // フォントデータを読み込む
    initializeFontdata();
  }
  // 描画対象の範囲外の文字コードが渡された
  if (ch < ' ' || '~' < ch)
  {
    // 何も描画しない
    return;
  }
  // コピーするフォント画像データの開始 x 座標を求める
  int fontImgBeginX = (ch - ' ') * fontWidth;

  for (int y = 0; y < fontHeight; y++)
  {
    for (int x = 0; x < fontWidth; x++)
    {
      int destX = destBeginX + x;
      int destY = destBeginY + y;
      if ((destX < 0) || (destX >= metainfo.Bmp_width) ||
        (destY < 0) || (destY >= metainfo.Bmp_height))
      {
        // 描画先がデータ範囲外
        continue;
      }

      // フォント画像を書き込み。白色は透明化
      // フォント画像が黒色に近い程、フォント色側に書き込み先の元色を近づけることで、印字を表現。
      ColorRGB fontBaseColor = fontImage->get(fontImgBeginX + x, y);
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
*/