#pragma once
#include <ndclibs.hpp>

using namespace nl;

namespace a3c
{
  /// @brief 地域の名称を扱う辞書 Entity
  class LabelsGeoEntity : public EntityBase
  {
  public:
    std::string EN;
    std::string JP;

  public:
    bool setData(std::vector<std::string> data) override
    {
      EN = data.at(1); // 英語名称
      JP = data.at(2); // 日本語名称
      return true;
    }
  };
}
