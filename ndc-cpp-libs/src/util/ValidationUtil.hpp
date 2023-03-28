#pragma once
#include <string>
#include <regex>
#include "StringUtil.hpp"

namespace nl
{
  namespace util
  {
    namespace _inner
    {
      /// @brief ±記号を表記可能な数字であることを判別する
      static const std::regex RegInt(R"([\+\-]?[0-9]+)");
    }

    template <class T>
    static inline bool isBetweenNotEq(const T min, const T max, const T checkVal)
    {
      return (min < checkVal && checkVal < max) ? true : false;
    }

    template <class T>
    static inline bool isBetweenEq(const T min, const T max, const T checkVal)
    {
      return (min <= checkVal && checkVal <= max) ? true : false;
    }

    /// @brief 数値であるかを判別
    /// @param[in] str 判定する文字列
    static inline bool isInt(const std::string &str)
    {
      return std::regex_match(str, _inner::RegInt);
    }

    /// @brief 数値(Float)であるかを判別
    /// @param[in] str 判定する文字列
    /// @return 数値(Float)なら true を返す
    static inline bool isFloat(const std::string &str)
    {
      float tmp = 0.0f;
      try
      {
        tmp = std::stof(str);
      }
      catch (std::invalid_argument &)
      {
        return false;
      }
      return true;
    }

    /// @brief 文字が "true" であれば treu を返す
    /// @param[in] str 判定する文字列
    /// @return 判定結果（ true or false )
    static inline bool isBool(const std::string &str)
    {
      std::string convStr = util::convToLowercase(str);
      return (convStr == "true" || convStr == "false") ? true : false;
    }
  }
}
