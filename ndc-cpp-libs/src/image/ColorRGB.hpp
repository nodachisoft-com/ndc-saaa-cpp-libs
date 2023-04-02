/**
 * @brief RGB�`���̉�f��������
 */
#pragma once

namespace nl
{

  /// @brief 1�s�N�Z��������� RGB �`���̉�f���
  class ColorRGB
  {
    public:
    /// @brief �ԐF�v�f
    unsigned char r;

    /// @brief �ΐF�v�f
    unsigned char g;

    /// @brief �F�v�f
    unsigned char b;

    ColorRGB()
      : r(0), g(0), b(0)
    {}

    ColorRGB(unsigned char r, unsigned char g, unsigned char b)
      : r(r), g(g), b(b)
    {
    }
  };
}