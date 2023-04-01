#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "NamedLocationNode.hpp"
#include "../baselayer/TerrainBaseLayerGenerator.hpp"

using namespace nl;

namespace a3c
{
  /// @brief
  ///  NamedLocation �� Node(�Ɋ܂܂��RouteGuide) , Edge ���Ǘ�����
  ///  Network �͗����Ȃǂ̑��݂̂��߁A��Ɏ��܂�Ƃ͌���Ȃ��B
  class NamedLocationMgr
  {
  private:
    /// @brief ���O�t���ꏊ�m�[�h�ꗗ
    std::vector<NamedLocationNode> namedLocationNodeList;


  public:
    // �R���X�g���N�^
    NamedLocationMgr() : namedLocationNodeList()
    {
    }

    /// <summary>
    /// NamedLocation �� Node ���Ȃ����l�b�g���[�N���`������
    /// </summary>
    void generateNetwork(TerrainBaseLayerGenerator& terrainBaseLayerGen);

    /// <summary>
    /// ���O�t���ꏊ�m�[�h���ꗗ�ɒǉ�
    /// </summary>
    /// <param name="node">�ǉ�����m�[�h</param>
    void addNamedLocationNodeList(NamedLocationNode node) {
      namedLocationNodeList.push_back(node);
    }


  public: // �A�N�Z�b�T�[

    /// <summary>
    /// ���ׂĂ̖��O�t���ꏊ�m�[�h�̈ꗗ���擾����
    /// </summary>
    /// <returns>���O�t���ꏊ�m�[�h�̈ꗗ</returns>
    std::vector<NamedLocationNode> getAllNamedLocationNodeList() {
      return namedLocationNodeList;
    }
  };
}