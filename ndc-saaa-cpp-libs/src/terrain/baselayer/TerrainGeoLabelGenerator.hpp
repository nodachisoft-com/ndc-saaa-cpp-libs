#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include <string>
#include "TerrainBaseConfig.hpp"
#include "../Biome.hpp"
#include "../../DBTables/DBs.hpp"

using namespace nl;

namespace a3c
{
  class TerrainGeoLabelGenerator
  {
  private:
    /// @brief 乱数の種
    int randomSeed;

    /// @brief 乱数生成器
    // MtRandomizer rnd;

    /// @brief データベースへの参照
    DBs *dbRef;

    // === 植生 (BIOME) ラベル ===
    /// @brief Biome_Deepsea_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeDeepseaLabelList;
    const std::string BIOME_DEEPSEA_LABEL_STARTWITH = "Biome_Deepsea_";

    /// @brief Biome_Sea_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeSeaLabelList;
    const std::string BIOME_SEA_LABEL_STARTWITH = "Biome_Sea_";

    /// @brief Biome_Seashore_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeSeashoreLabelList;
    const std::string BIOME_SEASHORE_LABEL_STARTWITH = "Biome_Seashore_";

    /// @brief Biome_Meadow_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeMeadowLabelList;
    const std::string BIOME_MEADOW_LABEL_STARTWITH = "Biome_Meadow_";

    /// @brief Biome_Forest_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeForestLabelList;
    const std::string BIOME_FOREST_LABEL_STARTWITH = "Biome_Forest_";

    /// @brief Biome_Desert_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeDesertLabelList;
    const std::string BIOME_DESERT_LABEL_STARTWITH = "Biome_Desert_";

    /// @brief Biome_Wasteland_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeWastelandLabelList;
    const std::string BIOME_WASTELAND_LABEL_STARTWITH = "Biome_Wasteland_";

    /// @brief Biome_Poison_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomePoisonLabelList;
    const std::string BIOME_POISON_LABEL_STARTWITH = "Biome_Poison_";

    /// @brief Biome_Mountain_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeMountainLabelList;
    const std::string BIOME_MOUNTAIN_LABEL_STARTWITH = "Biome_Mountain_";

    /// @brief Biome_Snow_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeSnowLabelList;
    const std::string BIOME_SNOW_LABEL_STARTWITH = "Biome_Snow_";

    /// @brief Biome_Mountainsnow_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> BiomeMountainsnowLabelList;
    const std::string BIOME_MOUNTAINSNOW_LABEL_STARTWITH = "Biome_Mountainsnow_";

    // === 大きさ (SCALE) ラベル ===

    /// @brief Scale_Small_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> ScaleSmallLabelList;
    const std::string SCALESMALL_LABEL_STARTWITH = "Scale_Small_";

    /// @brief Scale_Large_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> ScaleLargeLabelList;
    const std::string SCALELARGE_LABEL_STARTWITH = "Scale_Large_";

    // === ユニーク (UNIQUE) ラベル ===
    /// @brief U_ で始まる ID ラベル一覧
    std::vector<LabelsGeoEntity> UniqueLabelList;
    const std::string UNIQUE_LABEL_STARTWITH = "U_";

    /// @brief 指定した PK ではじまるラベル一覧を返す
    /// @param labelStartWith 前方一致検索する文字列 ID
    /// @return 一致した Entity 一覧
    std::vector<LabelsGeoEntity> getLabelsStartWith(std::string labelStartWith)
    {
      return dbRef->TABLE_LABELS_GEO.selectStartWithByPK(labelStartWith);
    }

    /// @brief ランダムに対象のリストから一つを取得する
    /// @param list ランダムに値を取得する元のリスト
    /// @return 取得した値
    LabelsGeoEntity getRndFromVecLabel(std::vector<LabelsGeoEntity> list, MtRandomizer &rnd)
    {
      size_t size = list.size();
      if (size == 0)
      {
        logger.error << "TerrainGeoLabelGenerator::getRndFromVec(): list.size() is 0." << std::endl;
      }
      int index = rnd.getRndInt(0, (int)size - 1);
      return list.at(index);
    }

    /// @brief ランダムに対象のリストから一つを取得する。取得した対象はリストから削除する
    /// @param list ランダムに値を取得する元のリスト
    /// @return 取得した値
    LabelsGeoEntity fetchRndFromVecLabel(std::vector<LabelsGeoEntity> &list, MtRandomizer &rnd)
    {
      size_t size = list.size();
      if (size == 0)
      {
        logger.error << "TerrainGeoLabelGenerator::fetchRndFromVecLabel(): list.size() is 0." << std::endl;
      }
      int index = rnd.getRndInt(0, (int)size - 1);
      LabelsGeoEntity result = list.at(index); // 要素を取り除く
      list.erase(std::cbegin(list) + index);
      return result;
    }

  public:
    TerrainGeoLabelGenerator() : dbRef(nullptr), ScaleSmallLabelList({}), randomSeed(0)
    {
    }
    bool init(const int randomSeed, DBs &dbRef);

    std::string generateLabel(const Biome &biome);
  };
}
