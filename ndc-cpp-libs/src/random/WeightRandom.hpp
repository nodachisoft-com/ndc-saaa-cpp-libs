
/**
 * @file WeightRandom.hpp
 * @brief 重み付きの要素をもったリストから重みの比率（確率）で要素を取り出す機能を提供する
 */
#pragma once
#include <vector>
#include "MtRandomizer.hpp"
#include "../exception/index.hpp"

namespace nl
{

  /**
   * @brief
   *  重み付きの要素をもったリストから重みの比率（確率）で要素を取り出す機能を持つクラス
   */
  template <class T>
  class WeightRandom
  {
  private:
    /// @brief 重みづけでオブジェクトを取得する際に使用する乱数生成器
    MtRandomizer rand;


    /// @brief 重みづけオブジェクト
    struct Elem
    {
        float perc;
        T contents;
    };
  private:
    /// @brief 重みづけオブジェクト格納先
    std::vector<Elem> list;

  public:
    /// @brief コンストラクタ
    WeightRandom() : rand(0)
    {
      list = std::vector<Elem>();
    }

    /// @brief 乱数シード付きで初期化
    /// @param randomSeed 乱数シード値
    void init(long randomSeed)
    {
      rand = MtRandomizer(randomSeed);
    }

    /// @brief 重みづけオブジェクトの追加
    /// @param perc 重み指定
    /// @param contents 追加するオブジェクト本体
    void push(const float perc, T contents)
    {
      list.push_back(Elem{perc, contents});
    }

    /// @brief 重みづけオブジェクトリストの長さ
    /// @return 長さ
    int length()
    {
      return list.size();
    }

    /// @brief 追加されたオブジェクト全体の重みの合計値を返す
    /// @return
    float getSumRandomPerc()
    {
      float sum = 0.0f;
      int len = length();
      for (int i = 0; i < len; i++)
      {
        Elem e = list.at(i);
        sum += e.perc;
      }
      return sum;
    }

    /// @brief 重みを考慮してランダムで登録されたオブジェクトから一つを取得する
    /// @return ランダムで取得したオブジェクト
    T getRandElem()
    {
      int len = length();
      float rndTarget = rand.getRndFloat(0.0f, getSumRandomPerc());
      float countRndSum = 0.0f;
      for (int i = 0; i < len; i++)
      {
        Elem e = list.at(i);
        if (countRndSum <= rndTarget && rndTarget < countRndSum + e.perc)
        {
          // 取得するランダムカウントの範囲に収まっている
          return e.contents;
        }
        countRndSum += e.perc;
      }
      throw TargetNotFoundException("WeightRandom#getRandElem Not Found Element.");
    }
  };
}
