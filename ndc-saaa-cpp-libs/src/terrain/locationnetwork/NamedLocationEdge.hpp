#pragma once
#include <iostream>
#include <ndclibs.hpp>

using namespace nl;

namespace a3c
{
  /// @brief
  ///  ���O�t���ꏊ�̃m�[�h�f�[�^�𐶐�����
  class NamedLocationEdge
  {
  private:

    /// @brief ���j�[�N�Ȗ��O�t���ꏊ�� ID
    int namedPlaceId;

    /// @brief Strategy Map ��̃��j�[�N�� Biome �ԍ�
    int refBiomeNo;

    /// @brief NamedLabel �̒��S�_
    Point2d centerPos;

    /// @brief Label��
    std::string labelName;

    // TODO: [  ] NamedPlaceRouteGuide ���쐬
    // -�ǂ� NamedPlace �ɂȂ��铹��
    // - �ǂ� NamedPlaceEdge �ւ̎Q�Ƃ�
    //     �� NamedPlaceEdge ���쐬�i�o�����l�b�g���[�N�j


  public:
    // �R���X�g���N�^
    NamedLocationEdge()
    {
    }

  };
}