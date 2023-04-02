#include "BitmapImage.hpp"


using namespace nl;

BitmapImage::BitmapImage(const std::string filename)
  :BitmapImage(filename.c_str())
{
}

BitmapImage::BitmapImage(const char *filename)
{
  // 8x8 サイズの画素で初期化する
  imgp = new ImageCanvas(8, 8);
  metainfo.setSize(8,8);
  ReadBmp(filename);
}

BitmapImage::BitmapImage(const int width, const int height)
{
  imgp = new ImageCanvas(width, height);
  metainfo.setSize(width, height);
}

BitmapImage::~BitmapImage()
{
  // 画像本体データを解放する
  delete imgp;
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
  unsigned char Bmp_headbuf[54] ={0};
  char Bmp_type[2]={0};
  int width = 0, height = 0;
  int Bmp_color = 0;
  fread(Bmp_headbuf, sizeof(unsigned char), BitmapMetainfo::HEADERSIZE, Bmp_Fp);

  memcpy(&Bmp_type, Bmp_headbuf, sizeof(metainfo.Bmp_type));
  if (strncmp(Bmp_type, "BM", 2) != 0)
  {
    fprintf(stderr, "Error: %s is not a bmp file.\n", filename);
    exit(1);
  }
  memcpy(&width, Bmp_headbuf + 18, sizeof(int));
  memcpy(&height, Bmp_headbuf + 22, sizeof(int));
  if (width * height > BitmapMetainfo::MAX_IMAGE_MEMORY)
  {
    fprintf(
      stderr,
      "Error: Image Size is too large. size=%d. Size Limit(X*Y)=%d\n",
      imgp->width * imgp->height,
      BitmapMetainfo::MAX_IMAGE_MEMORY);
    exit(1);
  }


  // キャンバスの削除
  delete imgp;

  // キャンバスの再確保
  imgp = new ImageCanvas( width, height );
  metainfo.setSize(width, height);

  memcpy(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
  if (Bmp_color != 24)
  {
    fprintf(stderr, "Error: Bmp_color = %d is not implemented in this program.\n", Bmp_color);
    exit(1);
  }

  int real_width = width * 3 + width % 4; // 4byte 境界にあわせるために実際の幅の計算

  // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
  if ((Bmp_Data = (unsigned char *)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for Bmp_Data!\n");
    exit(1);
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
      imgp->set(x,y, color);
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

  int Bmp_color = 24;
  int Bmp_header_size = metainfo.HEADERSIZE;
  int Bmp_info_header_size = 40;
  int Bmp_planes = 1;

  // 4byte 境界にあわせるために実際の幅の計算
  int real_width = metainfo.calcRealImagefileWidth();

  // 配列領域の動的確保. 失敗した場合はエラーメッセージを出力して終了
  if ((Bmp_Data = (unsigned char *)calloc(real_width, sizeof(unsigned char))) == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed for Bmp_Data!\n");
    exit(1);
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

      ColorRGB color = imgp->data.getWithIgnoreOutOfRangeData(x, y);
      Bmp_Data[x * 3] = color.b;
      Bmp_Data[x * 3+1] = color.g;
      Bmp_Data[x * 3+2] = color.r;
    }
    for (int j = metainfo.Bmp_width * 3; j < real_width; j++)
    {
      Bmp_Data[j] = 0;
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
  errno_t err = fopen_s(&Bmp_Fp, filename, "rb");
  if (err != 0) {
    fprintf(stderr, "Error: file %s couldn\'t open for write!.\n", filename);
    exit(1);
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
