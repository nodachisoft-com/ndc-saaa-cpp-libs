#pragma once
#include <string>

namespace nl
{
  class DebugFontData
  {
  private:
    std::string FONT_DATA;

  public:
    DebugFontData();
    ~DebugFontData();
    const int width = 1045;
    const int height = 21;
    unsigned char getGreyScale256(const int x, const int y);
  };
};
