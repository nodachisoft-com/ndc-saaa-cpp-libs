#include <string>
#include "../../pch.h"


using namespace nl;
using namespace std::literals::string_literals;

// カレントディレクトリのパスを取得する
TEST(CSVReader, curDir_case1)
{

  std::string csv_data = "apple,banana,orange\npear,grape,kiwi\\,mango\n";
  CSVReader reader(',', '\\', '#');
  reader.readCsv(csv_data);

  EXPECT_EQ("apple", reader.getCell(0, 0));
  EXPECT_EQ("banana", reader.getCell(1, 0));
  EXPECT_EQ("orange", reader.getCell(2, 0));

  EXPECT_EQ("pear", reader.getCell(0, 1));
  EXPECT_EQ("grape", reader.getCell(1, 1));
  EXPECT_EQ("kiwi\\,mango", reader.getCell(2, 1));
}
