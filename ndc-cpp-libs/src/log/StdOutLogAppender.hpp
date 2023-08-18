#pragma once
#include <string>
#include <unordered_map>
#include "ILogAppender.hpp"
#include "LOGLEVEL.hpp"
#include "LogHistory.hpp"

namespace nl
{
  class StdOutLogAppender : public ILogAppender
  {
    const std::unordered_map<LOGLEVEL, std::string> LOGLEVEL_STR = {
        {LOGLEVEL::DEBUG, "DEBUG"},
        {LOGLEVEL::INFO, "INFO"},
        {LOGLEVEL::WARN, "WARN"},
        {LOGLEVEL::ERROR, "ERROR"},
    };
    struct colorRGB
    {
      unsigned char r, g, b;
    };
    const std::unordered_map<LOGLEVEL, colorRGB> fontColorMap = {
        {LOGLEVEL::DEBUG, {100, 100, 150}},
        {LOGLEVEL::INFO, {0, 155, 255}},
        {LOGLEVEL::WARN, {175, 30, 0}},
        {LOGLEVEL::ERROR, {235, 30, 0}},
    };

    // エスケープシーケンスを標準出力する
    void escSeq(char const *controlSequence, char const function)
    {
      printf("\x1B[");
      printf("%s", controlSequence);
      putchar(function);
    }

    // RGB を指定して文字の色を変更する
    void setFontColorRGB(const colorRGB fontColor)
    {
      char buf[64];
      // sprintf(buf, "38;2;%d;%d;%d", fontColor.r, fontColor.g, fontColor.b);
      // sprintf_s(buf, sizeof(buf), "38;2;%d;%d;%d", fontColor.r, fontColor.g, fontColor.b);  // For Windows C Only
      
      // replace for C++
      std::sprintf(buf, "38;2;%d;%d;%d", fontColor.r, fontColor.g, fontColor.b);
      std::cout << buf << std::endl;
      escSeq(buf, 'm');
    }

    void resetColor()
    { // 色情報をリセット
      escSeq("0", 'm');
    }
    void outputLog(const LogHistory &log) override
    {

      colorRGB fontColor = fontColorMap.at(log.logLevel);
      setFontColorRGB(fontColor);
      std::cout
          << "[" << LOGLEVEL_STR.at(log.logLevel) << " " << log.timestamp << "] " << log.logMessage << std::endl;
      resetColor();
    }
  };
}
