#include "../../pch.h"
#include <ndclibs.hpp>
#include "../../ndc-saaa-cpp-libs/src/a3commonlib.hpp"


using namespace nl;
using namespace a3c;

// アプリDB の読み込みで TABLE_LABELS_GEO が 1000 件以上保持している
TEST(DBs, initialize_case001)
{
  DBs DBs;
  DBs.init();
  EXPECT_GT(DBs.TABLE_LABELS_GEO.size(), 1000);

  // 計算結果確認
  // EXPECT_EQ(0xed82cd11, result);
  // EXPECT_EQ(0xed82cd11, crc.getHash());
}
