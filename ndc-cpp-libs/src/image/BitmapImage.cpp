#include "BitmapImage.hpp"


using namespace nl;

BitmapImage::BitmapImage()
{
}

BitmapImage::~BitmapImage()
{
}

std::unique_ptr<ImageCanvas> BitmapImage::ReadBmp(const std::string filename)
{
  return ReadBmp(filename.c_str());
}

std::unique_ptr<ImageCanvas> BitmapImage::ReadBmp(const char* filename)
{
  FILE* Bmp_Fp;
  //errno_t err = fopen_s(&Bmp_Fp, filename, "rb"); // バイナリモード読み込み用にオープン
  // if (err != 0) {
  Bmp_Fp = fopen(filename,"rb");
  if ( Bmp_Fp == NULL ) {
    // ファイル読み込み失敗
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp File Open Failed. File=" << filename;
    throw IOException(ss.str());
  }
  unsigned char* Bmp_Data;              // 画像データを1行分格納


  // ヘッダ読み込み
  unsigned char Bmp_headbuf[54] = { 0 };
  char Bmp_type[2] = { 0 };
  int width = 0, height = 0;
  int Bmp_color = 0;
  fread(Bmp_headbuf, sizeof(unsigned char), BitmapMetainfo::HEADERSIZE, Bmp_Fp);

  memcpy(&Bmp_type, Bmp_headbuf, sizeof(metainfo.Bmp_type));
  if (strncmp(Bmp_type, "BM", 2) != 0)
  {
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp The file is not BMP file. File=" << filename;
    throw IOException(ss.str());
  }
  memcpy(&width, Bmp_headbuf + 18, sizeof(int));
  memcpy(&height, Bmp_headbuf + 22, sizeof(int));
  if (width * height > BitmapMetainfo::MAX_IMAGE_MEMORY)
  {
    // 読み込み画像サイズの上限を超えている場合
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp Too Large BMP file. "
      << "BMP file size is (" << width << "," << height << "). "
      << "Readable width * height limit is "
      << BitmapMetainfo::MAX_IMAGE_MEMORY << ". "
      << "File = " << filename;
    throw IOException(ss.str());
  }

  std::unique_ptr<ImageCanvas> imgp = std::make_unique<ImageCanvas>(width,height);
  metainfo.setSize(width, height);
  memcpy(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
  if (Bmp_color != 24)
  {
    // 24 ビットカラー以外の画像データは受け付けない
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp "
      << "only can read 24bit depth color. the file color depth=" << Bmp_color;
    throw IOException(ss.str());
  }
  // 4byte 境界にあわせるために実際の幅の計算
  int real_width = metainfo.calcRealImagefileWidth();

  
  if ((Bmp_Data = (unsigned char*)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp "
      << "Memory allocation failed for 1row of imagedata.";
    throw IOException(ss.str());
  }

  // 画像データ読み込み
  for (int i = 0; i < height; i++)
  {
    fread(Bmp_Data, 1, real_width, Bmp_Fp);
    for (int j = 0; j < width; j++)
    {
      int x = j;
      int y = height - i - 1; // BMP はデータと画像の Y 軸は反転
      ColorRGB color(Bmp_Data[x * 3], Bmp_Data[x * 3 + 1], Bmp_Data[x * 3 + 2]);
      imgp->set(x, y, color);
    }
  }

  // 動的に確保した配列領域の解放
  free(Bmp_Data);

  // ファイルクローズ
  fclose(Bmp_Fp);

  return imgp;
}


void BitmapImage::WriteBmp(const std::string filename,  ImageCanvas &canvas)
{
  WriteBmp(filename.c_str(), canvas);
}

void BitmapImage::WriteBmp(const char *filename,  ImageCanvas& canvas)
{
  FILE* Out_Fp;
  //errno_t err = fopen_s(&Out_Fp, filename, "wb");
  Out_Fp = fopen(filename, "wb");
  // if (err != 0) {
  if ( Out_Fp == NULL ) {
    // ファイル読み込み失敗
    std::stringstream ss;
    ss << "BitmapImage::WriteBmp File open failed as write-mode. File=" << filename;
    throw IOException(ss.str());
  }

  unsigned char* Bmp_Data; // 画像データを1行分格納

  int Bmp_color = 24;
  int Bmp_header_size = metainfo.HEADERSIZE;
  int Bmp_info_header_size = 40;
  int Bmp_planes = 1;

  // 4byte 境界にあわせるために実際の幅の計算
  metainfo.setSize( canvas.getWidth(), canvas.getHeight());
  int real_width = metainfo.calcRealImagefileWidth();

  if ((Bmp_Data = (unsigned char *)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
    std::stringstream ss;
    ss << "BitmapImage::ReadBmp "
      << "Memory allocation failed for 1row of imagedata.";
    throw IOException(ss.str());
  }

  uint8_t *buf = metainfo.getBmpFileHeader();

  // ヘッダ情報書き出し
  fwrite(buf, sizeof(unsigned char), metainfo.HEADERSIZE, Out_Fp);

  // 画像データ書き出し
  for (int i = 0; i < metainfo.Bmp_height; i++)
  {
    for (int j = 0; j < metainfo.Bmp_width; j++)
    {
      int x = j;
      int y = metainfo.Bmp_height - i - 1; // BMP はデータと画像の Y 軸は反転
      ColorRGB color = canvas.get(x,y);
      Bmp_Data[x * 3] = color.b;
      Bmp_Data[x * 3+1] = color.g;
      Bmp_Data[x * 3+2] = color.r;
    }
    fwrite(Bmp_Data, sizeof(unsigned char), real_width, Out_Fp);
  }
  free(Bmp_Data); // １行用のメモリ解放
  fclose(Out_Fp); // ファイルを閉じる

  delete buf;
}

void BitmapImage::PrintBmpInfo(const std::string filename)
{
  PrintBmpInfo(filename.c_str());
}

void BitmapImage::PrintBmpInfo(const char *filename)
{
  FILE* Bmp_Fp;
  // バイナリモード読み込み用にオープン
  //errno_t err = fopen_s(&Bmp_Fp, filename, "rb");
  Bmp_Fp = fopen(filename,"rb");
  if ( Bmp_Fp == NULL ) {
  // if (err != 0) {
    // ファイル読み込み失敗
    std::stringstream ss;
    ss << "BitmapImage::WriteBmp File open failed as read-mode. File=" << filename;
    throw IOException(ss.str());
  }

  unsigned char Bmp_headbuf[54] ={0};
  char Bmp_type[2] = { 0 };
  int width = 0, height = 0;
  int Bmp_color = 0;

  fread(Bmp_headbuf, sizeof(unsigned char), metainfo.HEADERSIZE, Bmp_Fp);

  memcpy(&Bmp_type, Bmp_headbuf, sizeof(Bmp_type));
  if (strncmp(Bmp_type, "BM", 2) != 0)
  {
    fprintf(stderr, "Error: %s is not a bmp file.\n", filename);
    exit(1);
  }
  /*
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
  */

  fclose(Bmp_Fp);
}
