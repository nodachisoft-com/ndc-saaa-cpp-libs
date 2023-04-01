#include "NamedLocationMgr.hpp"

using namespace a3c;

void NamedLocationMgr::generateNetwork(TerrainBaseLayerGenerator &terrainBaseLayerGen)
{   
  const std::vector<Biome> *biomeList = terrainBaseLayerGen.getBiomeList();

  // Biome �̒��S�n�� ���O�t���ꏊ�̈ꗗ�ɒǉ�����
  size_t biomeListLength =  biomeList->size();
  for (int i = 0; i < biomeListLength; i++) {
    Biome biome = biomeList->at(i);
    NamedLocationNode node(i, biome.biomeNo, biome.centerPos, "A");
    namedLocationNodeList.push_back( node );
  }

  // TODO: Network �� terrainBaseLayerGen �����Ɍv�Z���Đ�������
  Memory2d<float>* hf = terrainBaseLayerGen.getStrategyMapHF();



}

