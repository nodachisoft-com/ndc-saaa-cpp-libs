#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "../exception/index.hpp"

namespace nl
{
  class CSVReader
  {
  public:
    /// @brief CSV データ読み込み時のデリミタやエスケープコードの設定を行う
    /// @param delimiter CSV のデリミタ指定
    /// @param escape_char CSV セル内のエスケープコード
    /// @param comment_char CSV セルの頭にあれば読み込みをスキップする文字
    CSVReader(char delimiter, char escape_char, char comment_char) : delimiter_(delimiter), escape_char_(escape_char), comment_char_(comment_char) {}

    /// @brief
    ///   文字列データを CSV データとして読み込む。
    ///   行の先頭文字が "#" ならコメント行としてスキップする
    /// @param csv_data 読み込む CSVデータ
    /// @return
    void readCsv(const std::string &csv_data);

    /// @brief 指定したセルデータを読み取る。範囲外の場合は OutOfRangeAccessException 例外を返す
    /// @param col 列方向（x方向）の指定
    /// @param row 行方向（y方向）の指定
    /// @return 指定したセルデータの文字列
    std::string getCell(int col, int row);

    /// @brief 読み込み済みデータの行数を返す
    /// @return 辞書データの行数
    size_t rowsize()
    {
      return data.size();
    }

    /// @brief 対象の行のセルを文字列配列で返す
    /// @return 対象の行のセルを文字列配列
    std::vector<std::string> getRowCellList(int rowIndex);

  private:
    /// @brief 読み込み後のデータ
    std::vector<std::vector<std::string>> data;

    /// @brief デリミタ
    char delimiter_;

    /// @brief エスケープコード
    char escape_char_;

    /// @brief コメント行として無視する先頭文字
    char comment_char_;
  };
}
