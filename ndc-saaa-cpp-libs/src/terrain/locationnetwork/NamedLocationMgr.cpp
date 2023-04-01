#include "NamedLocationMgr.hpp"

using namespace a3c;

void NamedLocationMgr::generateNetwork(TerrainBaseLayerGenerator &terrainBaseLayerGen)
{   
  const std::vector<Biome> *biomeList = terrainBaseLayerGen.getBiomeList();

  // Biome の中心地を 名前付き場所の一覧に追加する
  size_t biomeListLength =  biomeList->size();
  for (int i = 0; i < biomeListLength; i++) {
    Biome biome = biomeList->at(i);
    NamedLocationNode node(i, biome.biomeNo, biome.centerPos, "A");
    namedLocationNodeList.push_back( node );
  }

  // TODO: Network を terrainBaseLayerGen を元に計算して生成する
  Memory2d<float>* hf = terrainBaseLayerGen.getStrategyMapHF();



}

