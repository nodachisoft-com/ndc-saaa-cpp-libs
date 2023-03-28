#pragma once
#include <vector>
#include <set>

namespace nl
{
  namespace util
  {

    /// @brief Vector 配列の中に重複する値が入っているかをチェックする
    /// @tparam 重複チェックする配列要素の型
    /// @param vec Vector 配列
    /// @return 重複があれば true を返す
    template <typename T>
    static inline bool isDuplicateElemInArray(const std::vector<T> &vec)
    {
      std::set<T> set;
      for (const T &elem : vec)
      {
        if (!set.insert(elem).second)
        {
          return true;
        }
      }
      return false;
    }

    /// @brief Vector 配列の中に指定した値が含まれるかをチェックする
    /// @tparam T 配列要素の型
    /// @param vec 配列
    /// @param val 検索する値
    /// @return 含まれていれば true
    template <typename T>
    static inline bool isExistInArray(const std::vector<T> &vec, const T val)
    {
      return std::find(vec.begin(), vec.end(), val) != vec.end();
    }

    /// @brief 文字列配列を文字の長い順番で降順で並び替える
    /// @param dirPathList 文字列の配列
    /// @return 並び替えした後の配列
    static inline std::vector<std::string> sortStringVectorByLengthDesc (std::vector<std::string> dirPathList) {
      std::sort(dirPathList.begin(), dirPathList.end(), [](const std::string& a, const std::string& b) {
        return a.size() > b.size();
      });
      return dirPathList;
    }
  }
}
