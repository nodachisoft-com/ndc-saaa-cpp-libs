/**
 * @brief BITMAP�t�@�C���̃w�b�_�����Ɋ܂܂�郁�^��������
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
    // �萔
    static const int HEADERSIZE = 54;
    static const int PALLETSIZE = 1024;

    // @brief 4K ���x���������\�ȏ���T�C�Y�Ƃ���
    static const int MAX_IMAGE_MEMORY = (1024 * 1080) * (4 * 2);

    constexpr static char Bmp_type[2] = {'B','M'};

    // ���^���
    // TODO: MetaInfo �\���̂ɓ��ꍞ�ނׂ�
    //unsigned char Bmp_headbuf[54];  /// @brief �w�b�_���i�[���邽�߂̕ϐ�
    //unsigned char Bmp_Pallet[1024]; /// @brief �J���[�p���b�g���i�[

    // char Bmp_type[2];                  /// @brief �t�@�C���^�C�v "BM"
    unsigned long Bmp_size;            /// @brief bmp�t�@�C���̃T�C�Y (�o�C�g)
    unsigned int Bmp_info_header_size; /// @brief ���w�b�_�̃T�C�Y = 40
    unsigned int Bmp_header_size;      /// @brief �w�b�_�T�C�Y = 54
    long Bmp_height;                   /// @brief ���� (�s�N�Z��)
    long Bmp_width;                    /// @brief ��   (�s�N�Z��)
    unsigned short Bmp_planes;         /// @brief �v���[���� ��� 1
    unsigned short Bmp_color;          /// @brief �F (�r�b�g)     24
    long Bmp_comp;                     /// @brief ���k���@         0
    long Bmp_image_size;               /// @brief �摜�����̃t�@�C���T�C�Y (�o�C�g)
    long Bmp_xppm;                     /// @brief �����𑜓x (ppm)
    long Bmp_yppm;                     /// @brief �����𑜓x (ppm)

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
    /// �摜�T�C�Y���Z�b�g
    /// </summary>
    /// <param name="width">�������i���j</param>
    /// <param name="height">�c�����i�����j</param>
    void setSize(int width, int height)
    {
      Bmp_width = width;
      Bmp_height = height;
      Bmp_image_size = HEADERSIZE + ( width * height * (Bmp_color / 8));
    }

    /// <summary>
    /// Bitmapfile �̉摜�v�f�̕��� 4 �o�C�g�P�ʂŐݒ�ł��]���� 0 �Ƃ���
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

