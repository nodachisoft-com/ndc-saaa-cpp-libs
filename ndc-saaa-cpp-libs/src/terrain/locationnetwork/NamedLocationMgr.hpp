#pragma once
#include <iostream>
#include <ndclibs.hpp>
#include "NamedLocationNode.hpp"
#include "../baselayer/TerrainBaseLayerGenerator.hpp"

using namespace nl;

namespace a3c
{
  /// @brief
  ///  NamedLocation の Node(に含まれるRouteGuide) , Edge を管理する
  ///  Network は離島などの存在のため、一つに収まるとは限らない。
  class NamedLocationMgr
  {
  private:
    /// @brief 名前付き場所ノード一覧
    std::vector<NamedLocationNode> namedLocationNodeList;


  public:
    // コンストラクタ
    NamedLocationMgr() : namedLocationNodeList()
    {
    }

    /// <summary>
    /// NamedLocation の Node をつなげたネットワークを形成する
    /// </summary>
    void generateNetwork(TerrainBaseLayerGenerator& terrainBaseLayerGen);

    /// <summary>
    /// 名前付き場所ノードを一覧に追加
    /// </summary>
    /// <param name="node">追加するノード</param>
    void addNamedLocationNodeList(NamedLocationNode node) {
      namedLocationNodeList.push_back(node);
    }


  public: // アクセッサー

    /// <summary>
    /// すべての名前付き場所ノードの一覧を取得する
    /// </summary>
    /// <returns>名前付き場所ノードの一覧</returns>
    std::vector<NamedLocationNode> getAllNamedLocationNodeList() {
      return namedLocationNodeList;
    }
  };
}