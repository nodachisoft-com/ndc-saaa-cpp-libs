/**
 * @brief BITMAPファイルのヘッダ部分に含まれるメタ情報を扱う
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

namespace nl
{
  class BitmapMetainfo
  {
  public:
    // 定数
    static const int HEADERSIZE = 54;
    static const int PALLETSIZE = 1024;

    // @brief 4K レベルを扱い可能な上限サイズとする
    static const int MAX_IMAGE_MEMORY = (1024 * 1080) * (4 * 2);

    constexpr static char Bmp_type[2] = {'B','M'};

    // メタ情報部
    // TODO: MetaInfo 構造体に入れ込むべき
    //unsigned char Bmp_headbuf[54];  /// @brief ヘッダを格納するための変数
    //unsigned char Bmp_Pallet[1024]; /// @brief カラーパレットを格納

    // char Bmp_type[2];                  /// @brief ファイルタイプ "BM"
    unsigned long Bmp_size;            /// @brief bmpファイルのサイズ (バイト)
    unsigned int Bmp_info_header_size; /// @brief 情報ヘッダのサイズ = 40
    unsigned int Bmp_header_size;      /// @brief ヘッダサイズ = 54
    long Bmp_height;                   /// @brief 高さ (ピクセル)
    long Bmp_width;                    /// @brief 幅   (ピクセル)
    unsigned short Bmp_planes;         /// @brief プレーン数 常に 1
    unsigned short Bmp_color;          /// @brief 色 (ビット)     24
    long Bmp_comp;                     /// @brief 圧縮方法         0
    long Bmp_image_size;               /// @brief 画像部分のファイルサイズ (バイト)
    long Bmp_xppm;                     /// @brief 水平解像度 (ppm)
    long Bmp_yppm;                     /// @brief 垂直解像度 (ppm)

    BitmapMetainfo() 
    : 
      // Bmp_headbuf({0})
      //,Bmp_Pallet({0})
      // Bmp_type({'B','M'})
       Bmp_size(0)
      ,Bmp_info_header_size(40)
      ,Bmp_header_size(54)
      ,Bmp_height(0)
      ,Bmp_width(0)
      ,Bmp_planes(1)
      ,Bmp_color(24)
      ,Bmp_comp(0)
      ,Bmp_image_size(0)
      ,Bmp_xppm(0)
      ,Bmp_yppm(0)
    {}

    /// <summary>
    /// 画像サイズをセット
    /// </summary>
    /// <param name="width">横方向（幅）</param>
    /// <param name="height">縦方向（高さ）</param>
    void setSize(int width, int height)
    {
      Bmp_width = width;
      Bmp_height = height;
      Bmp_image_size = HEADERSIZE + ( width * height * (Bmp_color / 8));
    }

    /// <summary>
    /// Bitmapfile の画像要素の幅は 4 バイト単位で設定でき余白は 0 とする
    /// </summary>
    /// <returns></returns>
    int calcRealImagefileWidth()
    {
      return Bmp_width * 3 + Bmp_width % 4;
    }


    // std::shared_ptr<uint8_t[]> getBmpFileHeader()
    uint8_t* getBmpFileHeader()
    {
      // uint8_t *buf = new uint8_t[HEADERSIZE];
      //std::shared_ptr<uint8_t[]> bufSharedPtr 
      //= std::make_shared<uint8_t[]>(HEADERSIZE ,0);

      int real_width = Bmp_width * 3 + Bmp_width % 4; // to method
      int bmpImageSize = Bmp_height * real_width;
      int bmpFileSize = bmpImageSize + HEADERSIZE;

      // uint8_t *buf = bufSharedPtr.get();
      uint8_t* buf = new uint8_t[HEADERSIZE]{0};
      buf[0] = 'B';
      buf[1] = 'M';

      memcpy(&buf[2], &bmpFileSize, sizeof(bmpFileSize));
      
      buf[6] = buf[7] = buf[8] = buf[9] = 0;
      memcpy(&buf[10], &Bmp_header_size, sizeof(Bmp_header_size));
      buf[11] = buf[12] = buf[13] = 0;
      memcpy(&buf[14], &Bmp_info_header_size, sizeof(Bmp_info_header_size));
      buf[15] = buf[16] = buf[17] = 0;

      memcpy(&buf[18], &Bmp_width, sizeof(Bmp_width));
      memcpy(&buf[22], &Bmp_height, sizeof(Bmp_height));
      memcpy(&buf[26], &Bmp_planes, sizeof(Bmp_planes));
      memcpy(&buf[28], &Bmp_color, sizeof(Bmp_color));
      memcpy(&buf[34], &Bmp_image_size, sizeof(Bmp_image_size));
      memcpy(&buf[38], &Bmp_xppm, sizeof(Bmp_xppm));
      memcpy(&buf[42], &Bmp_yppm, sizeof(Bmp_yppm));
      buf[46] = buf[47] = buf[48] = buf[49] = 0;
      buf[50] = buf[51] = buf[52] = buf[53] = 0;

      // return bufSharedPtr;
      return buf;
    }



  };
}

