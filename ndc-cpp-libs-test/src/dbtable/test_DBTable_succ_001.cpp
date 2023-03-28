#include "../../pch.h"


using namespace nl;
// using namespace std::literals::string_literals;
using namespace std::string_literals;

/// @brief テスト用の DBTable Entity
class TestEntity : public EntityBase
{
public:
  int i;
  float f;
  std::string s;
  bool b;

public:
  bool setData(std::vector<std::string> data) override
  {
    i = std::stoi(data.at(1));
    f = std::stof(data.at(2));
    s = data.at(3);
    b = (data.at(4) == "TRUE");
    return true;
  }
};

TEST(DBTable, readCSV_case001)
{
  // CSV データを定義
  std::string csv_data = "TEST_TABLE\n"s +
                         "ID,PRICE,SIZE,MESSAGE,BUYABLE\n"s +
                         "STRING,INT,FLOAT,STRING,BOOL\n"s +
                         "Apple,100,4.32,Hello,TRUE\n"s +
                         "Banana,-33,-45.67,I'm Test User!,FALSE"s;

  CSVReader reader(',', '\\', '#');
  reader.readCsv(csv_data);

  DBTable<TestEntity> testTable;
  EXPECT_EQ(testTable.readCSV(reader), 2); // 2行読み込み
  EXPECT_EQ(testTable.size(), 2);          // 2 行読み込み

  // Primary Key で検索
  std::vector<TestEntity> result = testTable.getByPK("Apple");
  EXPECT_EQ(result.size(), 1);
  TestEntity rec = result[0];
  EXPECT_EQ(rec.i, 100);
  EXPECT_EQ(rec.f, 4.32f);
  EXPECT_EQ(rec.s, "Hello");
  EXPECT_EQ(rec.b, true);
}

// コメント行が無視されることを確認する
TEST(DBTable, readCSV_case002)
{
  // CSV データを定義
  std::string csv_data = "TEST_TABLE\n"s +
                         "ID,PRICE,SIZE,MESSAGE,BUYABLE\n"s +
                         "STRING,INT,FLOAT,STRING,BOOL\n"s +
                         "# Apple,100,4.32,Hello,TRUE\n"s +
                         "Banana,-33,-45.67,I'm Test User!,FALSE"s;

  CSVReader reader(',', '\\', '#');
  reader.readCsv(csv_data);

  DBTable<TestEntity> testTable;
  EXPECT_EQ(testTable.readCSV(reader), 1); // 1 行読み込み
  EXPECT_EQ(testTable.size(), 1);          // 1 行読み込み

  // Primary Key で検索
  std::vector<TestEntity> result = testTable.getByPK("Banana");
  EXPECT_EQ(result.size(), 1);
  TestEntity rec = result[0];
  EXPECT_EQ(rec.i, -33);
  EXPECT_EQ(rec.f, -45.67f);
  EXPECT_EQ(rec.s, "I'm Test User!");
  EXPECT_EQ(rec.b, false);
}

// SelectStartWith で前方一致検索できることを確認する
// Entity から列名を指定して文字列での値を取得できることを確認する
TEST(DBTable, readCSV_SelectStartWith_case001)
{
  // CSV データを定義
  std::string csv_data = "TEST_TABLE\n"s +
                         "ID,PRICE,SIZE,MESSAGE,BUYABLE\n"s +
                         "STRING,INT,FLOAT,STRING,BOOL\n"s +
                         "Apple,100,4.32,Hello,TRUE\n"s +
                         "Banana,-33,-45.67,I'm Test User!,FALSE\n"s +
                         "Maple,55,0.33,Yeah,FALSE\n"s +
                         "PotateBurn,77,0.11,OOOH,TRUE\n"s +
                         "Blueberry,1,0.004,EYE,FALSE"s;

  CSVReader reader(',', '\\', '#');
  reader.readCsv(csv_data);

  DBTable<TestEntity> testTable;
  EXPECT_EQ(testTable.readCSV(reader), 5); // 5 行読み込み
  EXPECT_EQ(testTable.size(), 5);          // 5 行読み込み

  // Primary Key で検索
  std::vector<TestEntity> result = testTable.selectStartWithByPK("B");
  EXPECT_EQ(result.size(), 2); // Banana, Blueberry

  result = testTable.selectStartWithByPK("Potate");
  EXPECT_EQ(result[0].getData("MESSAGE"), "OOOH");
}
