#pragma once
#include <iostream>
#include <ndclibs.hpp>

using namespace nl;

namespace a3c
{
  /// @brief
  ///  World Map のベースとなる生成のための各種パラメータを定義
  ///  TODO: json などで外部から取り込み可能な UnorderedMap 形式の KeyValue データを持つ
  class TerrainBaseConfig
  {

  public:
    /// @brief 生成の種で利用
    int Seed;

    /// @brief 自然発生する Biome の種類数を指定
    unsigned char NaturalBiomeTypes;

    /// @brief World 生成時の SMALLEST サイズでの Biome 分割数について（x, y）成分それぞれを指定
    int BiomeDivisionSmallestCount;

    /// @brief
    ///   World 生成時の SMALLEST サイズでの MapUnit のサイズを規定。
    ///   関連する値は以下で計算する
    ///   ・ World 全体でのマップチップサイズは、この値に scale（整数）と MapUnitSize をかけ合わせた数
    ///   ・ World 全体のストラテジー解析用サイズは、この値に scale（整数）をかけ合わせた数
    Size2d WorldSmallestMapunitSize;

    /// @brief SMALLEST サイズに対しての、生成する World のスケールサイズを指定。(x, y) はそれぞれ整数を指定する
    int WorldScale;

    /// @brief
    ///  1 つのマップユニットが持つ x 成分と y 成分の マップチップ数
    ///  例）32 なら、1つのマップユニットは 32 * 32 のマップチップを持つ
    int MapUnitChipSize;

    /// @brief SMALLEST サイズベースでマップ端からの海の範囲を指定。[0.0f-0.5f] で指定する
    float mapEdgeWide;

  public:
    /// @brief Config をデフォルト値で初期化する
    TerrainBaseConfig();

    void init(const int Seed, const unsigned char NaturalBiomeTypes,
              const int BiomeDivisionSmallestCount,
              const Size2d WorldSmallestMapunitSize,
              const int WorldScale,
              const int MapUnitChipSize,
              const float mapEdgeWide);

    /// @brief World 全体でのマップチップサイズを取得する
    /// @return World 全体でのマップチップサイズ
    Size2d getWorldMapchipSize()
    {
      return WorldSmallestMapunitSize * WorldScale * MapUnitChipSize;
    }

    /// @brief World でのストラテジー解析用マップサイズを取得する
    /// @return World でのストラテジー解析用マップサイズ
    Size2d getStrategyResolutionSize()
    {
      return WorldSmallestMapunitSize * WorldScale;
    }
  };
}
