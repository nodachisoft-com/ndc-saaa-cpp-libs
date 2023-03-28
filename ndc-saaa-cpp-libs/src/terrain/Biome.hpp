#pragma once

using namespace nl;

namespace a3c
{
  class Biome
  {
  public:
    static const int BIOME_DEEPSEA = 0;       // 深海
    static const int BIOME_SEA = 1;           // 浅瀬
    static const int BIOME_SEASHORE = 2;      // 海岸の砂浜
    static const int BIOME_MEADOW = 3;        // 草原 ( voronoi type = 0 )
    static const int BIOME_FOREST = 4;        // 森 ( voronoi type = 1 )
    static const int BIOME_DESERT = 5;        // 砂漠 ( voronoi type = 2 )
    static const int BIOME_WASTELAND = 6;     // 荒地 ( voronoi type = 3 )
    static const int BIOME_POISONED = 7;      // 化学汚染 ( voronoi type = 4 )
    static const int BIOME_MOUNTAIN = 8;      // 山岳
    static const int BIOME_SNOW = 9;          // 雪
    static const int BIOME_MOUNTAINSNOW = 10; // 山の雪

  public:
    /// @brief Strategy Map 上のユニークな Biome 番号
    int biomeNo;

    /// @brief Biome の種類
    int biomeKind;

    /// @brief Biome の面積
    int biomeAreaSize;

    /// @brief Biome の中心点
    Point2d centerPos;

    /// @brief Label名
    std::string labelName;

  public:
    Biome() : biomeNo(0), biomeKind(0), biomeAreaSize(0), centerPos(0.0f, 0.0f),
              labelName(""){};
  };
}
