#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace nl
{
  namespace util
  {
    /// @brief 含まれる英字を大文字に変換して新しい文字列として結果を返す
    /// @param str 変換する元の文字列
    /// @return 大文字に変換した文字列
    static inline std::string convToUppercase(const std::string &str)
    {
      std::string result;
      result.resize(str.size());
      std::transform(str.begin(), str.end(), result.begin(), toupper);
      return result;
    }

    /// @brief 含まれる英字を小文字に変換して新しい文字列として結果を返す
    /// @param str 変換する元の文字列
    /// @return 小文字に変換した文字列
    static inline std::string convToLowercase(const std::string &str)
    {
      std::string result;
      result.resize(str.size());
      std::transform(str.begin(), str.end(), result.begin(), tolower);
      return result;
    }
  }
}
