#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../exception/index.hpp"
#include "../util/ValidationUtil.hpp"

using namespace std::literals;

namespace nl
{
  struct ColumnDataMap
  {
    char type;
    int index;
  };

  class EntityBase
  {

  public:
    std::string _pk;

    std::unordered_map<std::string, std::string> dataMap;

  public:
    EntityBase()
    {
    }

    virtual bool setData(std::vector<std::string> data)
    {
      throw TargetNotFoundException("No Impl Entity");
    }

    /// @brief 列名に紐づけてデータを文字列型で格納する。列名検索時に利用可能。
    /// @param columnNameList カラム名称一覧
    /// @param columnTypeList カラム型の一覧
    /// @param rowdata データ本体
    /// @return 正常にデータ設定できたら true
    bool setDataToColumnData(std::vector<std::string> columnNameList,
                             std::vector<std::string> columnTypeList,
                             std::vector<std::string> rowdata)
    {
      size_t size = columnNameList.size();
      _pk = rowdata[0]; // 必須項目の Primary Key 値にセット
      for (size_t i = 0; i < size; i++)
      {
        std::string key = columnNameList[i];
        // if (dataMap.count(key) != 0)
        // {
        //   // 検査：キー重複を検知
        //   throw ArgumentValidatioinException(
        //       "Column Duplicate Detected! ColumnName=["s + key + "]"s);
        // }
        if (!validateDataConvertibleFromString(columnTypeList[i], rowdata[i]))
        {
          return false;
        }
        dataMap[key] = rowdata[i];
      }
      return true;
    }

    /// @brief 指定した列名（カラム）が Entity に存在するかを確認する
    /// @param columnName 列名
    /// @return 存在する場合 true を返す
    bool isExistData(std::string columnName)
    {
      return dataMap.count(columnName) == 0 ? false : true;
    }

    /// @brief 対象の列名のデータを文字列型で取得する。列名が存在しない場合は空文字を返す
    /// @param columnName 列名
    /// @return 列名のデータを返す
    std::string getData(std::string columnName)
    {
      if (!isExistData(columnName))
      {
        return "";
      }
      return dataMap[columnName];
    }

    /// @brief Primary Key を取得する
    /// @return Primary Key
    std::string getPK()
    {
      return _pk;
    }

    /// @brief Primary Key をセットする
    /// @param pk[in] Primary Key
    void setPK(const std::string pk)
    {
      _pk = pk;
    }

  private:
    /// @brief 渡されたデータの型定義に、データの文字列が変換可能かを検査する
    /// @param type 型定義 'S' = STRING, 'I' = INT , 'F' = FLOAT, 'B' = BOOL
    /// @param data データ文字列
    /// @return 変換可能であれば true を返す
    bool validateDataConvertibleFromString(std::string type, std::string data)
    {
      if (type.size() == 0)
      {
        return false;
      }
      char typeChar = type.c_str()[0];
      if (typeChar == 'S')
      {
        // STRING 型
        return true;
      }
      else if (typeChar == 'I')
      {
        if (!util::isInt(data)) // INT 型だが、int に変換不可能
        {
          logger.error << "DBTable Not INTEGER data. data=[" << data << "]" << std::endl;
          // Logger logger;
          // logger.errorLog("DBTable Not INTEGER data. data=["s + data + "]"s);
          return false;
        }
      }
      else if (typeChar == 'F')
      {
        if (!util::isFloat(data)) // FLOAT 型だが、float に変換不可能
        {
          logger.error << "DBTable Not FLOAT data. data=[" << data << "]" << std::endl;
          // Logger logger;
          // logger.errorLog("DBTable Not FLOAT data. data=["s + data + "]"s);
          return false;
        }
      }
      else if (typeChar == 'B')
      {
        if (!util::isBool(data)) // BOOL 型だが、bool に変換不可能
        {
          logger.error << "DBTable Not BOOL data. data=[" << data << "]" << std::endl;
          // Logger logger;
          // logger.errorLog("DBTable Not BOOL data. data=["s + data + "]"s);
          return false;
        }
      }
      else
      {
        // 渡されたデータ型が不明
        logger.error << "DBTable Unknown Type. type=[" << type << "]" << std::endl;
        // Logger logger;
        // logger.errorLog("DBTable Unknown Type. type=["s + type + "]"s);
        return false;
      }

      return true;
    }
  };
}
