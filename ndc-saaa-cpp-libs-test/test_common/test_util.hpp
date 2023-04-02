#pragma once
#include <vector>
#include <ndclibs.hpp>
#include "../../ndc-saaa-cpp-libs/src/a3commonlib.hpp"

using namespace nl;
using namespace a3c;

namespace TestUtil
{
  static ColorRGB getColorByHeight(float height)
  {
    struct Stratum
    {
      float hFrom, hTo;
      ColorRGB cFrom, cTo;
    };

    std::vector<Stratum> stratumList{
        // 高さ（深い～浅い）, 色（深い～浅い）
        {0.0f, 100.0f, {0, 87, 232}, {43, 73, 249}},        // 海（深海）
        {100.0f, 120.0f, {43, 73, 249}, {120, 214, 251}},   // 海（浅瀬）
        {120.0f, 123.0f, {255, 252, 147}, {255, 239, 130}}, // 砂浜
        {123.0f, 136.0f, {194, 241, 143}, {174, 222, 50}},  // 草原
        {136.0f, 140.0f, {174, 222, 50}, {162, 166, 106}},  // 山と平野の堺
        {140.0f, 145.0f, {162, 166, 106}, {149, 139, 123}}, // 山岳
        {145.0f, 200.0f, {149, 139, 123}, {236, 236, 236}}, // 山頂
        {200.0f, 255.0f, {236, 236, 236}, {245, 246, 246}}, // 山頂（雪）
    };

    // 条件漏れを目視できるように赤色
    ColorRGB result{255, 0, 0};

    size_t stratumListLen = stratumList.size();
    for (size_t i = 0; i < stratumListLen; i++)
    {
      Stratum st = stratumList[i];
      if (st.hFrom <= height && height < st.hTo)
      {
        float alpha = (height - st.hFrom) / (st.hTo - st.hFrom);
        result.r = (unsigned char)(st.cTo.r * alpha + st.cFrom.r * (1.0f - alpha));
        result.g = (unsigned char)(st.cTo.g * alpha + st.cFrom.g * (1.0f - alpha));
        result.b = (unsigned char)(st.cTo.b * alpha + st.cFrom.b * (1.0f - alpha));
        break;
      }
    }

    return result;
  }

  /// @brief 生成された HeightField を視覚的に確認するためのテスト用画像出力関数
  /// @param hf HeightField を格納した2次元データ。高さ情報として 0.0F-255.0F が格納される
  /// @param filepath 画像出力先のパス
  /// @return 生成した画像データの CRC32 を返す
  static unsigned long drawHfColorful(
      Memory2d<float> *hf, const std::string filepath)
  {
    int width = hf->getWidth();
    int height = hf->getHeight();
    BitmapImage image(width, height);
    ImageCanvas* canvas = image.getRefImageCanvas();
    Crc32 crc;
    for (int v = 0; v < height; v++)
    {
      for (int u = 0; u < width; u++)
      {
        float res = hf->getWithIgnoreOutOfRangeData(u, v);
        ColorRGB color = getColorByHeight(res);
        canvas->set(u, v, color);
        crc.calcUpdate(color.r);
        crc.calcUpdate(color.g);
        crc.calcUpdate(color.b);
      }
    }
    image.WriteBmp(filepath);
    return crc.getHash();
  }

  /// @brief 生成された BiomeType を視覚的に確認するためのテスト用画像出力関数
  /// @param biomeType BiomeType を格納した2次元データ。0 以上の Biome 種類が格納されている
  /// @param filepath 画像出力先のパス
  /// @return 生成した画像データの CRC32 を返す
  static unsigned long drawBiomeNoColorful(
      Memory2d<short> *biomeType, const std::string filepath, std::vector<Biome> *refBiomeList, int biomeLabelSizeLargerThan)
  {
    std::vector<ColorRGB> colorList = {
        {0, 0, 142},     // 青 : 深海 : 0
        {66, 189, 249},  // 水 : 浅瀬 : 1
        {255, 252, 147}, // 黄 : 海岸の砂浜 : 2
        {194, 241, 143}, // 緑 : 草原 : 3
        {4, 241, 143},   // 濃緑 : 森 : 4
        {238, 182, 13},  // オレンジ : 砂漠 : 5
        {119, 80, 21},   // 濃い茶色 : 荒地 : 6
        {129, 45, 138},  // 紫 : 化学汚染 : 7
        {99, 107, 75},   // 灰色 : 山岳 : 8
        {181, 255, 252}, // 水 : 雪 : 9
        {243, 243, 243}, // 白 : 山の雪 : 10

    };

    int width = biomeType->getWidth();
    int height = biomeType->getHeight();
    BitmapImage image(width, height);
    ImageCanvas* canvas = image.getRefImageCanvas();
    Crc32 crc;
    for (int v = 0; v < height; v++)
    {
      for (int u = 0; u < width; u++)
      {
        short biomeTypeNo = biomeType->getWithIgnoreOutOfRangeData(u, v);
        ColorRGB color = colorList[biomeTypeNo];
        canvas->set(u, v, color);
        crc.calcUpdate(color.r);
        crc.calcUpdate(color.g);
        crc.calcUpdate(color.b);
      }
    }
    // BIOME LABEL の記述
    ColorRGB COLOR_RED{(unsigned char)255, (unsigned char)0, (unsigned char)0};
    size_t biomeSize = refBiomeList->size();
    for (size_t i = 0; i < biomeSize; i++)
    {
      Biome biome = refBiomeList->at(i);
      if (biome.biomeAreaSize > biomeLabelSizeLargerThan)
      {
        canvas->writeText((int)biome.centerPos.x, (int)biome.centerPos.y, biome.labelName, COLOR_RED);
      }
    }
    image.WriteBmp(filepath);
    return crc.getHash();
  }
}
