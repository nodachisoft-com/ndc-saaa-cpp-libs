/**
 * @brief �摜�̃L�����o�X��������
 */
#pragma once

#include <string>
#include <iostream>
#include "DebugFontData.hpp"
#include "ColorRGB.hpp"

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

    /// @brief ���L�����t�H���g�f�[�^
    static ImageCanvas *fontCanvas;

    static int sharedCountForFontCanvas;

  public:
    ImageCanvas(const int width, const int height)
      : width(width), height(height)
    {
      data = (ColorRGB*)calloc(width * height, sizeof(ColorRGB));
      initDefaultFontData();
    }

    ~ImageCanvas()
    {
      free(data);
      sharedCountForFontCanvas--;
      if (sharedCountForFontCanvas == 0 )
      {
        // ImageCanvas �I�u�W�F�N�g�̗��p�҂� 0 �ɂȂ����� static ���e���폜
        delete fontCanvas;
      }
    }

    void set(int x, int y, ColorRGB& color);

    ColorRGB get(int x, int y);

    void clear(ColorRGB& color);

    void writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color);

    void writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color);

    void initDefaultFontData();

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


