/**
 * @brief �摜�̃L�����o�X��������
 */
#pragma once

#include <string>
#include <iostream>
//#include "BitmapImage.hpp"
#include "ColorRGB.hpp"
//#include "DebugFontData.hpp"


namespace nl
{

  /// @brief 2�����̉摜�L�����o�X�������N���X
  class ImageCanvas
  {
  public:
    /// @brief ����
    int height;

    /// @brief ��
    int width;

    /// @brief �f�[�^�{��
    ColorRGB* data;

  public:
    ImageCanvas(const int width, const int height)
      : width(width), height(height)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
    }

    ~ImageCanvas()
    {
      free(data);
    }


    void set(int x, int y, ColorRGB& color);


    ColorRGB get(int x, int y);

    void clear(ColorRGB& color);


  public: // �A�N�Z�T
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


