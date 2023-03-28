#include "BitmapImage.hpp"

using namespace nl;

BitmapImage::BitmapImage(const std::string filename)
{
  BitmapImage(filename.c_str());
}

BitmapImage::BitmapImage(const char *filename)
{
  // 8x8 サイズの画素で初期化する
  ImageDataStruct imageDataStruct{
      8, 8, NULL};
  imgp = imageDataStruct;
  ReadBmp(filename);
}

BitmapImage::BitmapImage(const int width, const int height)
{
  imgp.width = width;
  imgp.height = height;
  Bmp_width = width;
  Bmp_height = height;
  Bmp_image_size = width * height * 3;
  for (int i = 0; i < HEADERSIZE; i++)
  {
    Bmp_headbuf[i] = 0;
  }
  imgp.data = (ColorRGB *)calloc(width * height, sizeof(ColorRGB));
}

BitmapImage::~BitmapImage()
{
  // 画像本体データを解放する
  free(imgp.data);
  imgp.data = NULL;

  if (fontImage != NULL)
  {
    // フォントデータ読み込み済みなら解放する
    delete fontImage;
  }
}

void BitmapImage::ReadBmp(const std::string filename)
{
  ReadBmp(filename.c_str());
}

void BitmapImage::ReadBmp(const char *filename)
{
  FILE* Bmp_Fp;
  errno_t err = fopen_s(&Bmp_Fp, filename, "rb"); // バイナリモード読み込み用にオープン
  if (err != 0) {
      fprintf(stderr, "Error: file %s couldn\'t open for read!.\n", filename);
      exit(1);
  }
  unsigned char *Bmp_Data;              // 画像データを1行分格納


  // ヘッダ読み込み
  fread(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Bmp_Fp);

  memcpy(&Bmp_type, Bmp_headbuf, sizeof(Bmp_type));
  if (strncmp(Bmp_type, "BM", 2) != 0)
  {
    fprintf(stderr, "Error: %s is not a bmp file.\n", filename);
    exit(1);
  }
  memcpy(&imgp.width, Bmp_headbuf + 18, sizeof(imgp.width));
  memcpy(&imgp.height, Bmp_headbuf + 22, sizeof(imgp.height));
  if (imgp.width * imgp.height > MAX_IMAGE_MEMORY)
  {
    fprintf(stderr, "Error: Image Size is too large. size=%d. Size Limit(X*Y)=%d\n", imgp.width * imgp.height, MAX_IMAGE_MEMORY);
    exit(1);
  }

  if (imgp.data != NULL)
  {
    // 前回のバッファが存在する場合は解放
    free(imgp.data);
  }
  imgp.data = (ColorRGB *)malloc(sizeof(ColorRGB) * imgp.width * imgp.height);

  memcpy(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
  if (Bmp_color != 24)
  {
    fprintf(stderr, "Error: Bmp_color = %d is not implemented in this program.\n", Bmp_color);
    exit(1);
  }

  int real_width = imgp.width * 3 + imgp.width % 4; // 4byte 境界にあわせるために実際の幅の計算

  // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
  if ((Bmp_Data = (unsigned char *)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for Bmp_Data!\n");
    exit(1);
  }

  // 画像データ読み込み
  for (int i = 0; i < imgp.height; i++)
  {
    fread(Bmp_Data, 1, real_width, Bmp_Fp);
    for (int j = 0; j < imgp.width; j++)
    {
      int x = j;
      int y = imgp.height - i - 1; // BMP はデータと画像の Y 軸は反転
      int pos = imgp.width * y + x;
      imgp.data[pos].b = Bmp_Data[x * 3];
      imgp.data[pos].g = Bmp_Data[x * 3 + 1];
      imgp.data[pos].r = Bmp_Data[x * 3 + 2];
    }
  }

  // 動的に確保した配列領域の解放
  free(Bmp_Data);

  // ファイルクローズ
  fclose(Bmp_Fp);
}

void BitmapImage::WriteBmp(const std::string filename)
{
  WriteBmp(filename.c_str());
}

void BitmapImage::WriteBmp(const char *filename)
{
  FILE* Out_Fp;
  errno_t err = fopen_s(&Out_Fp, filename, "wb");

  if (err != 0) {
    fprintf(stderr, "Error: file %s couldn\'t open for read!.\n", filename);
    exit(1);
  }

  unsigned char* Bmp_Data; // 画像データを1行分格納

  Bmp_color = 24;
  Bmp_header_size = HEADERSIZE;
  Bmp_info_header_size = 40;
  Bmp_planes = 1;

  int real_width = imgp.width * 3 + imgp.width % 4; // 4byte 境界にあわせるために実際の幅の計算

  // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
  if ((Bmp_Data = (unsigned char *)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for Bmp_Data!\n");
    exit(1);
  }

  // ヘッダ情報作成
  Bmp_xppm = Bmp_yppm = 0;
  Bmp_image_size = imgp.height * real_width;
  Bmp_size = Bmp_image_size + HEADERSIZE;
  Bmp_headbuf[0] = 'B';
  Bmp_headbuf[1] = 'M';
  memcpy(Bmp_headbuf + 2, &Bmp_size, sizeof(Bmp_size));
  Bmp_headbuf[6] = Bmp_headbuf[7] = Bmp_headbuf[8] = Bmp_headbuf[9] = 0;
  memcpy(Bmp_headbuf + 10, &Bmp_header_size, sizeof(Bmp_header_size));
  Bmp_headbuf[11] = Bmp_headbuf[12] = Bmp_headbuf[13] = 0;
  memcpy(Bmp_headbuf + 14, &Bmp_info_header_size, sizeof(Bmp_info_header_size));
  Bmp_headbuf[15] = Bmp_headbuf[16] = Bmp_headbuf[17] = 0;

  memcpy(Bmp_headbuf + 18, &imgp.width, sizeof(imgp.width));
  memcpy(Bmp_headbuf + 22, &imgp.height, sizeof(imgp.height));
  memcpy(Bmp_headbuf + 26, &Bmp_planes, sizeof(Bmp_planes));
  memcpy(Bmp_headbuf + 28, &Bmp_color, sizeof(Bmp_color));
  memcpy(Bmp_headbuf + 34, &Bmp_image_size, sizeof(Bmp_image_size));
  memcpy(Bmp_headbuf + 38, &Bmp_xppm, sizeof(Bmp_xppm));
  memcpy(Bmp_headbuf + 42, &Bmp_yppm, sizeof(Bmp_yppm));
  Bmp_headbuf[46] = Bmp_headbuf[47] = Bmp_headbuf[48] = Bmp_headbuf[49] = 0;
  Bmp_headbuf[50] = Bmp_headbuf[51] = Bmp_headbuf[52] = Bmp_headbuf[53] = 0;

  // ヘッダ情報書き出し
  fwrite(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Out_Fp);

  // 画像データ書き出し
  for (int i = 0; i < imgp.height; i++)
  {
    for (int j = 0; j < imgp.width; j++)
    {
      int x = j;
      int y = imgp.height - i - 1; // BMP はデータと画像の Y 軸は反転
      int pos = imgp.width * y + x;
      Bmp_Data[x * 3] = imgp.data[pos].b;
      Bmp_Data[x * 3 + 1] = imgp.data[pos].g;
      Bmp_Data[x * 3 + 2] = imgp.data[pos].r;
    }
    for (int j = imgp.width * 3; j < real_width; j++)
    {
      Bmp_Data[j] = 0;
    }
    fwrite(Bmp_Data, sizeof(unsigned char), real_width, Out_Fp);
  }
  free(Bmp_Data); // １行用のメモリ解放
  fclose(Out_Fp); // ファイルを閉じる
}

void BitmapImage::PrintBmpInfo(const std::string filename)
{
  PrintBmpInfo(filename.c_str());
}

void BitmapImage::PrintBmpInfo(const char *filename)
{
  FILE* Bmp_Fp;
  errno_t err = fopen_s(&Bmp_Fp, filename, "rb");
  //FILE *Bmp_Fp = fopen(filename, "rb"); // バイナリモード読み込み用にオープン
  if (err != 0) {
    fprintf(stderr, "Error: file %s couldn\'t open for write!.\n", filename);
    exit(1);
  }
  /*
  FILE *Bmp_Fp = fopen(filename, "rb");
  if (Bmp_Fp == NULL)
  {
    fprintf(stderr, "Error: file %s couldn\'t open for write!\n", filename);
    exit(1);
  }
  */

  fread(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Bmp_Fp);

  memcpy(&Bmp_type, Bmp_headbuf, sizeof(Bmp_type));
  if (strncmp(Bmp_type, "BM", 2) != 0)
  {
    fprintf(stderr, "Error: %s is not a bmp file.\n", filename);
    exit(1);
  }
  memcpy(&Bmp_size, Bmp_headbuf + 2, sizeof(Bmp_size));
  memcpy(&Bmp_width, Bmp_headbuf + 18, sizeof(Bmp_width));
  memcpy(&Bmp_height, Bmp_headbuf + 22, sizeof(Bmp_height));
  memcpy(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
  memcpy(&Bmp_comp, Bmp_headbuf + 30, sizeof(Bmp_comp));
  memcpy(&Bmp_image_size, Bmp_headbuf + 34, sizeof(Bmp_size));
  memcpy(&Bmp_xppm, Bmp_headbuf + 38, sizeof(Bmp_xppm));
  memcpy(&Bmp_yppm, Bmp_headbuf + 42, sizeof(Bmp_yppm));

  printf("Filename   = %s \n", filename);
  printf("FileType   = %c%c \n", Bmp_type[0], Bmp_type[1]);
  printf("FileSize   = %ld (byte)\n", Bmp_size);
  printf("Width      = %ld (pixel)\n", Bmp_width);
  printf("Height     = %ld (pixel)\n", Bmp_height);
  printf("Color Depth= %d (bit)\n", Bmp_color);
  printf("Compress   = %ld\n", Bmp_comp);
  printf("Image size = %ld (byte)\n", Bmp_image_size);
  printf("Height     = %ld (ppm)\n", Bmp_xppm);
  printf("Width      = %ld (ppm)\n", Bmp_yppm);

  fclose(Bmp_Fp);
}

void BitmapImage::set(int x, int y, ColorRGB &color)
{
  int pos = imgp.width * y + x;
  imgp.data[pos].b = color.b;
  imgp.data[pos].g = color.g;
  imgp.data[pos].r = color.r;
}

ColorRGB BitmapImage::get(int x, int y)
{
  int pos = imgp.width * y + x;
  ColorRGB result;
  result.b = imgp.data[pos].b;
  result.g = imgp.data[pos].g;
  result.r = imgp.data[pos].r;
  return result;
}

long BitmapImage::getWidth()
{
  return imgp.width;
}

long BitmapImage::getHeight()
{
  return imgp.height;
}

void BitmapImage::clear(ColorRGB &color)
{
  unsigned char r = color.r;
  unsigned char g = color.g;
  unsigned char b = color.b;
  int size = imgp.width * imgp.height;
  for (int i = 0; i < size; i++)
  {
    imgp.data[i].b = b;
    imgp.data[i].g = g;
    imgp.data[i].r = r;
  }
}

// フォントデータ読み込み
bool BitmapImage::initializeFontdata()
{
  // fontImage = new BitmapImage("src/image/debugfont.bmp");

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

void BitmapImage::writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB &color)
{
  size_t size = text.size();
  for (size_t index = 0; index < size; index++)
  {
    const char drawCh = text[index];
    writeChar(destBeginX + index * fontWidth, destBeginY, drawCh, color);
  }
}

void BitmapImage::writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB &color)
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
      if ((destX < 0) || (destX >= imgp.width) ||
          (destY < 0) || (destY >= imgp.height))
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
