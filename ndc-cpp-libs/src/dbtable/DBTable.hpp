#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "../csv/CSVReader.hpp"
#include "EntityBase.hpp"
#include "../exception/NdcBaseException.hpp"
#include "../log/index.hpp"

namespace nl
{
  template <class XxxEntity>
  class DBTable
  {
  private:
    /// @brief テーブル名称を定義
    std::string tablename;

    /// @brief 辞書データを格納する
    std::unordered_map<std::string, XxxEntity> data;

    /// @brief カラム名一覧を格納する
    std::vector<std::string> columnNameList;

    /// @brief カラムのデータ型定義（ STRING, INT, FLOAT, BOOL )
    std::vector<std::string> columnTypeList;

  public:
    /// @brief 初期化
    DBTable() : tablename(), data({}), columnNameList({}), columnTypeList({})
    {
    }

    /// @brief CSV データを辞書データとして読み込む
    /// @param csv 読み込むデータ元
    /// @return 読み込んだデータ行。重複などのエラー検知データは対象外。
    int readCSV(CSVReader &csv)
    {
      // Logger logger;
      size_t rowsize = csv.rowsize();
      if (rowsize <= 2)
      {
        // 検証：DB データとして読み込むためのメタ情報が足りていない
        throw ArgumentValidatioinException("csv data is too short.");
      }
      // テーブル名取得
      tablename = csv.getRowCellList(0).at(0);
      if (tablename.size() == 0)
      {
        // 検証：DB データとして読み込むテーブル名が空
        throw ArgumentValidatioinException("tablename is too Empty.");
      }

      // カラム定義一覧を取得
      columnNameList = csv.getRowCellList(1);
      size_t columnNameListSize = columnNameList.size();

      // カラムのデータ型定義を取得
      columnTypeList = csv.getRowCellList(2);
      size_t columnTypeListSize = columnTypeList.size();

      // 検証：カラム定義一覧とカラムデータ型一覧の数が不一致
      if (columnNameListSize != columnTypeListSize)
      {
        throw ArgumentValidatioinException(
            "ColumnNameList.size() != ColumnTypeList.size()"s + " Tablename=["s + tablename + "]"s);
      }
      // 検証：カラム名が存在しない
      if (columnNameListSize == 0)
      {
        throw ArgumentValidatioinException("ColumnNameList is Empty!"s + " Tablename=["s + tablename + "]"s);
      }
      // 検証：カラム名に重複がないかチェック
      if (util::isDuplicateElemInArray(columnNameList))
      {
        throw ArgumentValidatioinException("Column Duplicate Detected!"s);
      }

      int insertCount = 0;
      for (size_t i = 3; i < rowsize; i++)
      {
        std::vector<std::string> rowdata = csv.getRowCellList((int)i);
        size_t rowDataSize = rowdata.size();
        if (rowDataSize == 0)
        {

          // データが存在しない
          logger.error << "DBTable::readCSV rowdata.size()=0."
                       << " Tablename=[" << tablename << "] "
                       << " CSV LINE=" << i << " th." << std::endl;

          continue;
        }

        if (rowDataSize < columnNameListSize)
        {
          // データの数がカラムの数未満であり、不足している。行データを出力する
          std::string msg = "DBTable::readCSV rowdata.size()="s +
                            std::to_string(rowDataSize) +
                            ". too short. "s +
                            " Tablename=["s + tablename + "] "s;
          msg.append("rowdata=[");
          for (int j = 0; j < rowDataSize; i++)
          {
            msg.append(std::to_string(j));
            if (j < rowDataSize - 1)
            {
              msg.append(",");
            }
            msg.append("]");
          }
          logger.error << msg << std::endl;

          continue;
        }

        std::string pk = rowdata[0];
        if (isPkDataExist(pk))
        {
          logger.error << "PK Duplicate. PK=[" << pk << "]" << std::endl;
          continue;
        }
        XxxEntity entity;
        entity.setData(rowdata);
        bool isValid = entity.setDataToColumnData(columnNameList, columnTypeList, rowdata);
        if (!isValid)
        {
          // データを Entity に変換できなかった
          logger.error << "Could'nt Convert to Entity. PK=[" << pk << "]" << std::endl;
          continue;
        }
        insertCount++;
        data[pk] = entity;
      }
      return insertCount;
    }

    /// @brief Primary Key をもつデータがテーブル内に存在するかを判定する
    /// @param pk Primary Key
    /// @return Primary Key データが存在する場合 true を返す
    bool isPkDataExist(std::string pk)
    {
      return data.count(pk) != 0 ? true : false;
    }

    size_t size()
    {
      return data.size();
    }

    /// @brief Primary Key が完全一致するデータをテーブル内から取得する
    /// @param pk Primary Key
    /// @return Primary Key データが存在する場合、対象データを一見返す
    std::vector<XxxEntity> getByPK(std::string pk)
    {
      std::vector<XxxEntity> result;
      if (isPkDataExist(pk))
      {
        result.push_back(data.at(pk));
      }
      return result;
    }

    /// @brief テーブル名を返す
    /// @return テーブル名
    std::string getTablename()
    {
      return tablename;
    }

    /// @brief Primary Key が前方一致するデータをテーブル内から取得する
    /// @param pkStartWith Primary Key に対して前方一致検索する文字列
    /// @return 条件に一致する Entity のリスト
    std::vector<XxxEntity> selectStartWithByPK(std::string pkStartWith)
    {
      std::vector<XxxEntity> result;
      for (auto itr = data.begin(); itr != data.end(); ++itr)
      {
        std::string keyName(itr->first);

        if (keyName.size() >= pkStartWith.size() &&
            std::equal(std::begin(pkStartWith), std::end(pkStartWith), std::begin(keyName)))
        {
          result.push_back(data[keyName]);
        }
      }
      return result;
    }
  };
}
