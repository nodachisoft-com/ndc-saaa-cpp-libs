/**
 * @brief �摜�̃L�����o�X��������
 */
#pragma once

#include <string>
#include <iostream>
#include "../memory_bank/Memory2d.hpp"
#include "DebugFontData.hpp"
#include "ColorRGB.hpp"

namespace nl
{

  /// @brief 2�����̉摜�L�����o�X�������N���X
  class ImageCanvas
  {
  public:

    /// @brief �f�[�^�{��
    Memory2d<ColorRGB> data;

    /// @brief ���L�����t�H���g�f�[�^
    static ImageCanvas *fontCanvas;

    /// <summary>
    /// ImageCanvas �̃C���X�^���X���ɉ������J�E���g�Ǘ�
    /// fontCanvas ��1 ��ڌĂяo�����̏������A�S�폜����ɗ��p
    /// </summary>
    static int sharedCountForFontCanvas;

  public:
    ImageCanvas(const int width, const int height)
      : data(width, height, ColorRGB(0, 0, 0))
    {
      initDefaultFontData();
    }

    ~ImageCanvas()
    {
      sharedCountForFontCanvas--;
      if (sharedCountForFontCanvas == 0 )
      {
        // ImageCanvas �I�u�W�F�N�g�̗��p�҂� 0 �ɂȂ����� static ���e���폜
        delete fontCanvas;
      }
    }

    /// @brief �F�����o�b�t�@�ɏ�������
    /// @param[in] x �������މ摜���̈ʒu�i�������_�Ƃ���j
    /// @param[in] y �������މ摜���̈ʒu�i������_�Ƃ���j
    /// @param[in] color �������ސF���iRGB�j
    void set(int x, int y, ColorRGB& color)
    {
      data.setWithIgnoreOutOfRangeData(x, y, color);
    }


    /// @brief �w��̉摜�ʒu����F�����擾����
    /// @param[in] x �������މ摜���̈ʒu�i�������_�Ƃ���j
    /// @param[in] y �������މ摜���̈ʒu�i������_�Ƃ���j
    /// @return �擾�����F���iRGB�j
    ColorRGB get(int x, int y)
    {
      return data.getWithIgnoreOutOfRangeData(x, y);
    }

    /// @brief �摜���w��̐F�œh��Ԃ�
    /// @param[in] color �h��Ԃ��F
    void clear(ColorRGB& color)
    {
      data.setWholeData(color);
    }

    /// @brief
    ///  �����t�H���g�i1����������11px�A���� 21px�j�ŕ�����`�悷��B
    ///  ���p���鏉��Ƀt�H���g�摜����ǂݍ��ށB
    /// @param[in] x ������`�悷��J�n�ʒu x ���W
    /// @param[in] y ������`�悷��J�n�ʒu y ���W
    /// @param[in] text �`�悷�镶����
    /// @param[in] color �t�H���g�̐F���iRGB�j
    void writeText(const int destBeginX, const int destBeginY, const std::string text, ColorRGB& color);

    /// @brief
    ///  �����t�H���g�i1����������11px�A���� 21px�j�łP������`�悷��B
    ///  ���p���鏉��Ƀt�H���g�摜����ǂݍ��ށB
    /// @param[in] x ������`�悷��J�n�ʒu x ���W
    /// @param[in] y ������`�悷��J�n�ʒu y ���W
    /// @param[in] oneText �`�悷�镶��
    /// @param[in] color �t�H���g�̐F���iRGB�j
    void writeChar(const int destBeginX, const int destBeginY, const char ch, ColorRGB& color);

    /// <summary>
    /// �t�H���g�f�[�^������������
    /// </summary>
    void initDefaultFontData();

  public: // �A�N�Z�T

    /// @brief �摜�̉������s�N�Z�������擾����
    int getWidth()
    {
      return data.getWidth();
    }

    /// @brief �摜�̏c�����s�N�Z�������擾����
    int getHeight()
    {
      return data.getHeight();
    }
  };
};


