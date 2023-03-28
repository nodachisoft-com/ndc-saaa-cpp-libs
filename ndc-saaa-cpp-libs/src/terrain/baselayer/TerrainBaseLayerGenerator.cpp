#include "TerrainBaseLayerGenerator.hpp"

using namespace a3c;
using namespace nl;
using namespace std::string_literals;

TerrainBaseLayerGenerator::TerrainBaseLayerGenerator()
{
  biomeList = std::vector<Biome>();
}

TerrainBaseLayerGenerator::~TerrainBaseLayerGenerator()
{
}

void TerrainBaseLayerGenerator::generateBaseTerrain()
{
  // Map の最もベースとなる基本的な Height Field を作成する
  generateStrategyMapHF();

  // Strategy レベルで BiomeNo 割り振りデータを生成する
  generateStrategyMapBiomType();

  // Map 上で一定以上の Biome サイズに対してユニークな ID および名称を割り振る
  generateStrategyMapBiomeId();
}

float TerrainBaseLayerGenerator::getLayerHfAsWorldmapchip_StrategyLevel(const float u, const float v)
{
  int mapScale = conf.WorldScale;
  int layer1noisePointX = 1 * mapScale, layer1noisePointY = 1 * mapScale;
  int layer2noisePointX = 2 * mapScale, layer2noisePointY = 2 * mapScale;
  int layer3noisePointX = 3 * mapScale, layer3noisePointY = 3 * mapScale;
  int layer4noisePointX = 6 * mapScale, layer4noisePointY = 6 * mapScale;

  float wholeMapchipSizeWidth = conf.getWorldMapchipSize().x;
  float wholeMapchipSizeHeight = conf.getWorldMapchipSize().y;

  // 基本地形生成
  float res1 = layer1.pnoise2(
      (u * layer1noisePointX) / wholeMapchipSizeWidth,
      (v * layer1noisePointY) / wholeMapchipSizeHeight,
      layer1noisePointX, layer1noisePointY, 255.0f);

  float res2 = layer2.pnoise2(
      (u * layer2noisePointX) / wholeMapchipSizeWidth,
      (v * layer2noisePointY) / wholeMapchipSizeHeight,
      layer2noisePointX, layer2noisePointY, 255.0f);

  float res3 = layer3.pnoise2(
      (u * layer3noisePointX) / wholeMapchipSizeWidth,
      (v * layer3noisePointY) / wholeMapchipSizeHeight,
      layer3noisePointX, layer3noisePointY, 255.0f);

  float res4 = layer4.pnoise2(
      (u * layer4noisePointX) / wholeMapchipSizeWidth,
      (v * layer4noisePointY) / wholeMapchipSizeHeight,
      layer4noisePointX, layer4noisePointY, 255.0f);
  float hBase = res1 * 0.5f + res2 * 0.3f + res3 * 0.15f + res4 * 0.05f;

  // Edge Filter を使って外洋を作成
  float edgeFilterFix = edgeFileter.pos2(
      u / wholeMapchipSizeWidth,
      v / wholeMapchipSizeHeight);
  float hBaseFilter = hBase * edgeFilterFix;

  return hBaseFilter;
}

void TerrainBaseLayerGenerator::generateStrategyMapHF()
{
  // Continents/Biom などの基本的な地理情報を 1024 x 1024 で生成し勢力図、経路情報の生成に使用する
  Size2d resSize = conf.getStrategyResolutionSize();
  int width = (int)resSize.x;
  int height = (int)resSize.y;

  // HeightField 作成
  strategyMapHF.init(width, height, 0.0f);
  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      float res = getLayerHfAsWorldmapchip_StrategyLevel(
          (float)(u * conf.MapUnitChipSize),
          (float)(v * conf.MapUnitChipSize));

      strategyMapHF.setWithIgnoreOutOfRangeData(u, v, res);
    }
  }
}

void TerrainBaseLayerGenerator::generateStrategyMapBiomType()
{
  Size2d resSize = conf.getStrategyResolutionSize();
  int width = (int)resSize.x;
  int height = (int)resSize.y;
  int biomeDiv = conf.WorldScale * conf.BiomeDivisionSmallestCount;

  // TODO: リテラル値は最終的に Config で外部から挿入できるようにする
  // 地層（Stratum）定義
  int STRATUM_DEEPSEA = 0;      // 深海
  int STRATUM_SEA = 1;          // 浅瀬
  int STRATUM_SEASHORE = 2;     // 海岸の砂浜
  int STRATUM_PLAIN = 3;        // 様々なバイオームが存在する領域
  int STRATUM_MOUNTAIN = 4;     // 山岳
  int STRATUM_SNOWMOUNTAIN = 5; // 高山（雪）
  // 地層を適用する高さ（HeightField の値）
  std::vector<float> stratumLv{
      100.0f,
      120.0f,
      124.0f,
      150.0f,
      180.0f,
      999.0f};

  strategyMapBiomType.init((int)resSize.x, (int)resSize.y, 0);
  for (int v = 0; v < height; v++)
  {
    // 緯度を 0.0f ～ 1.0f の範囲で計算
    float latitude = calcLatitude(v / (float)height);

    for (int u = 0; u < width; u++)
    {
      float b = biom.pos2((biomeDiv * u) / (float)width, (biomeDiv * v) / (float)height);
      float height = strategyMapHF.getWithIgnoreOutOfRangeData(u, v);
      // 北極南極に近く雪の地域
      bool highLatitudeSnow = (height * 0.5f + 128.0f) * latitude >= 150.0f;

      // HF の高さにより、選択可能な Biome を分別する
      if (height < stratumLv[STRATUM_DEEPSEA])
      {
        strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_DEEPSEA);
      }
      else if (height < stratumLv[STRATUM_SEA])
      {
        strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_SEA);
      }
      // 陸上の Biome
      else if (height < stratumLv[STRATUM_SEASHORE])
      {
        if (highLatitudeSnow) // 北極南極の近く
        {
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_SNOW);
        }
        else
        {
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_SEASHORE);
        }
      }
      else if (height < stratumLv[STRATUM_PLAIN])
      {
        if (highLatitudeSnow) // 北極南極の近く
        {
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_SNOW);
        }
        else
        {
          // 平地で様々な Biome が発生する
          int biomeType = (int)(b + Biome::BIOME_MEADOW);
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, biomeType);
        }
      }
      else if (height < stratumLv[STRATUM_MOUNTAIN])
      {
        if (highLatitudeSnow) // 北極南極に近く、雪山バイオーム
        {
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_MOUNTAINSNOW);
        }
        else
        {
          // 山岳バイオーム
          strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_MOUNTAIN);
        }
      }
      else if (height < stratumLv[STRATUM_SNOWMOUNTAIN])
      {
        strategyMapBiomType.setWithIgnoreOutOfRangeData(u, v, Biome::BIOME_MOUNTAINSNOW);
      }
    }
  }
}

void TerrainBaseLayerGenerator::generateStrategyMapBiomeId()
{
  struct Point
  {
    int x, y;
  };

  Size2d resSize = conf.getStrategyResolutionSize();
  int width = (int)resSize.x;
  int height = (int)resSize.y;

  // RandomSeed
  MtRandomizer rnd(conf.Seed);

  // MapBiom のユニークな ID を採番していく。
  // 一定以上のエリア（塗りつぶし面積）
  short biomeIdCount = 1;
  strategyMapBiomeId.init((int)resSize.x, (int)resSize.y, 0);

  for (int v = 0; v < height; v++)
  {
    for (int u = 0; u < width; u++)
    {
      if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(u, v) != 0)
      {
        // 既に検査済み
        continue;
      }

      // 塗りつぶし開始点候補リスト
      std::vector<Point> checkNextArea{};
      int drawCount = 0;

      // 重心を求めるための計算用変数(x成分)
      float sumX = 0;
      float sumY = 0;

      // 検査する対象の色を取得し塗りつぶし検査対象リストに加える
      short targetBiomeNo = strategyMapBiomType.getWithIgnoreOutOfRangeData(u, v);
      checkNextArea.push_back({u, v});

      while (!checkNextArea.empty())
      {
        Point &lastp = checkNextArea.back();
        Point p;
        p.x = lastp.x;
        p.y = lastp.y;

        if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(p.x, p.y) != 0)
        {
          // 既に検査済み
          checkNextArea.pop_back(); // 末尾要素を削除
          continue;
        }
        sumX += p.x; // 重心計算用 x 成分を加算
        sumY += p.y; // 重心計算用 y 成分を加算

        checkNextArea.pop_back(); // 末尾要素を削除

        drawCount++;
        strategyMapBiomeId.setWithIgnoreOutOfRangeData(p.x, p.y, biomeIdCount);

        // 上が未検査かつ同じ Biome種類 かを調べる
        if (p.y > 0)
        {
          // 同じ Biome 種類かを確認
          if (targetBiomeNo == strategyMapBiomType.getWithIgnoreOutOfRangeData(p.x, p.y - 1))
          {
            // 未検査かを確認
            if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(p.x, p.y - 1) == 0)
            {
              // 未検査である場合、検査対象リストに追加する
              checkNextArea.push_back({p.x, p.y - 1});
            }
          }
        }
        // 左が未検査かつ同じ Biome 種類かを調べる
        if (p.x > 0)
        {
          // 同じ Biome 種類かを確認
          if (targetBiomeNo == strategyMapBiomType.getWithIgnoreOutOfRangeData(p.x - 1, p.y))
          {
            // 未検査かを確認
            if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(p.x - 1, p.y) == 0)
            {
              // 未検査である場合、検査対象リストに追加する
              checkNextArea.push_back({p.x - 1, p.y});
            }
          }
        }
        // 右が未検査かつ同じ Biome 種類かを調べる
        if (p.x < width - 1)
        {
          if (targetBiomeNo == strategyMapBiomType.getWithIgnoreOutOfRangeData(p.x + 1, p.y))
          {
            // 同じ Biome 種類
            if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(p.x + 1, p.y) == 0)
            {
              // 未検査である場合、検査対象リストに追加する
              checkNextArea.push_back({p.x + 1, p.y});
            }
          }
        }
        // 下が未検査かつ同じ Biome 種類かを調べる
        if (p.y < height - 1)
        {
          if (targetBiomeNo == strategyMapBiomType.getWithIgnoreOutOfRangeData(p.x, p.y + 1))
          {
            // 同じ Biome 種類
            if (strategyMapBiomeId.getWithIgnoreOutOfRangeData(p.x, p.y + 1) == 0)
            {
              // 未検査である場合、検査対象リストに追加する
              checkNextArea.push_back({p.x, p.y + 1});
            }
          }
        }
      }
      // 一定以上の Biome は一覧に記録する
      if (drawCount > 20)
      {
        Biome biome;
        biome.biomeNo = biomeIdCount;
        biome.biomeKind = targetBiomeNo;
        biome.biomeAreaSize = drawCount;
        biome.centerPos = nl::Point2d(sumX / (float)drawCount, sumY / (float)drawCount);
        biome.labelName = terrainGeoLabelGenerator.generateLabel(biome);

        biomeList.push_back(biome);
      }

      biomeIdCount++;
    }
  }
}

void TerrainBaseLayerGenerator::init(TerrainBaseConfig _conf, DBs &_dbRef)
{
  conf = _conf;
  dbRef = &_dbRef;

  int biomeDivisionCount = _conf.BiomeDivisionSmallestCount * _conf.WorldScale;
  biom.init(_conf.Seed, _conf.NaturalBiomeTypes, biomeDivisionCount, biomeDivisionCount);
  layer1.init(_conf.Seed);
  layer2.init(_conf.Seed + 66);
  layer3.init(_conf.Seed + 100);
  layer4.init(_conf.Seed + 103);
  edgeFileter.init(_conf.mapEdgeWide / _conf.WorldScale);
  terrainGeoLabelGenerator.init(_conf.Seed, _dbRef);
}
