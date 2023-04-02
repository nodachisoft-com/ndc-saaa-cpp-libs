/**
 * @brief �摜�̃L�����o�X��������
 */
#pragma once

#include <string>
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
      : width(width), height(height), data(nullptr)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
    }

    ~ImageCanvas()
    {
      freeResources();
    }

    /// <summary>
    /// ���\�[�X������������
    /// </summary>
    void freeResources()
    {
      free(data);
    }
  };
};


