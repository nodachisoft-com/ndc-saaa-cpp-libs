#include "TerrainBaseConfig.hpp"

using namespace a3c;

TerrainBaseConfig::TerrainBaseConfig()
{
  init(0, 4, 8, {64, 64}, 1, 32, 0.2f);
};

void TerrainBaseConfig::init(const int _Seed, const unsigned char _NaturalBiomeTypes,
                             const int _BiomeDivisionSmallestCount,
                             const Size2d _WorldSmallestMapunitSize,
                             const int _WorldScale,
                             const int _MapUnitChipSize,
                             const float _mapEdgeWide)
{
  Seed = _Seed;
  NaturalBiomeTypes = _NaturalBiomeTypes;
  BiomeDivisionSmallestCount = _BiomeDivisionSmallestCount;
  WorldSmallestMapunitSize = _WorldSmallestMapunitSize;
  WorldScale = _WorldScale;
  MapUnitChipSize = _MapUnitChipSize;
  mapEdgeWide = _mapEdgeWide;
}
