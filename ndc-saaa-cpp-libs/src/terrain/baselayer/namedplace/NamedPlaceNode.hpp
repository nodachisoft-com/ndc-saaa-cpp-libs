#pragma once
#include <iostream>
#include <ndclibs.hpp>

using namespace nl;

namespace a3c
{
  /// @brief
  ///  名前付き場所のノードデータを生成する
  class NamedPlaceNode
  {
  private:

    /// @brief ユニークな名前付き場所の ID
    int namedPlaceId;

    /// @brief Strategy Map 上のユニークな Biome 番号
    int refBiomeNo;

    /// @brief NamedLabel の中心点
    Point2d centerPos;

    /// @brief Label名
    std::string labelName;

    // TODO: [  ] NamedPlaceRouteGuide を作成
    // -どの NamedPlace につながる道か
    // - どの NamedPlaceEdge への参照か
    //     → NamedPlaceEdge を作成（双方向ネットワーク）


  public:
    // コンストラクタ
    NamedPlaceNode(int refBiomeNo, Point2d centerPos, std::string labelName)
      : refBiomeNo(refBiomeNo), centerPos(centerPos), labelName(labelName)
    {
    }

  };
}