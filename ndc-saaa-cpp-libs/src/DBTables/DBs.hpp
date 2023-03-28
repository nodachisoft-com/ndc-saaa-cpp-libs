#pragma once
#include <ndclibs.hpp>
#include <string>
#include "LabelsGeoEntity.hpp"

using namespace nl;

namespace a3c
{

  /// @brief アプリ内の読み込み専用データをまとめて管理する
  class DBs
  {
  private:
    bool initialized;
    std::string dbdataPath;

  public: // データ一覧
    /// @brief 地理ラベルを格納するテーブルデータ
    DBTable<LabelsGeoEntity> TABLE_LABELS_GEO;

  public:
    DBs();
    bool init();
  };
}
