#include "../../pch.h"

using namespace nl;

// ２次元で確保したメモリのサイズを取得する
TEST(Memory2d, size_succ001)
{
  Memory2d<int> mem(4, 3, 999);
  mem.setOutOfRangeData(-1);
  EXPECT_EQ(4*3, mem.size());
}

// ２次元で確保したメモリに getWithIgnoreOutOfRangeData を行い
// 範囲外を指定した時に OutOfRangeData で設定した値が返ること
// 範囲内を指定した時に 初期化で指定した値が返ること
TEST(Memory2d, getWithIgnoreOutOfRangeData_succ001)
{
  Memory2d<int> mem(4, 3, 999);
  mem.setOutOfRangeData(-55);

  // 範囲外のバッファを指定したら -55 が返る
  EXPECT_EQ(-55, mem.getWithIgnoreOutOfRangeData(100,100));
  EXPECT_EQ(-55, mem.getWithIgnoreOutOfRangeData(-1,-1));

  // 範囲内のバッファを指定した
  EXPECT_EQ(999, mem.getWithIgnoreOutOfRangeData(2,2));
}

// ２次元で確保したメモリに setWithIgnoreOutOfRangeData で値をセット
// 範囲内の setWithIgnoreOutOfRangeData で true が返ること
// 範囲外の setWithIgnoreOutOfRangeData で false が返ること
TEST(Memory2d, setWithIgnoreOutOfRangeData_succ001)
{
  Memory2d<int> mem(500, 300, 0);
  mem.setOutOfRangeData(-1);
  bool setResult = true;
  for (int v = 0; v < 300; v++)
  {
    for (int u = 0; u < 500; u++)
    {
      if (mem.setWithIgnoreOutOfRangeData(u, v, 99) == false)
      {
        // 失敗した場合
        setResult = false;
      }
    }
  }

  // すべての範囲内と想定されるバッファ内容に値をセット出来たこと
  EXPECT_EQ(setResult, true);

  // 範囲外に set しようとして false が返る
  EXPECT_EQ(mem.setWithIgnoreOutOfRangeData(-5, 100, 100), false);
  EXPECT_EQ(mem.setWithIgnoreOutOfRangeData(500, 5, 100), false);
}

TEST(Memory2d, getDataPerodic_succ001)
{
  Memory2d<int> mem(3, 2, 111);
  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      mem.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
    }
  }

  // 通常の範囲内
  EXPECT_EQ(mem.getDataPerodic(0,0), 0);
  EXPECT_EQ(mem.getDataPerodic(1,0), 1);
  EXPECT_EQ(mem.getDataPerodic(2,0), 2);

  // 正の方向で範囲を超過している
  EXPECT_EQ(mem.getDataPerodic(3,0), 0);
  EXPECT_EQ(mem.getDataPerodic(4,0), 1);
  EXPECT_EQ(mem.getDataPerodic(5,0), 2);

  // 正の方向で範囲を超過している（２週目）
  EXPECT_EQ(mem.getDataPerodic(6,0), 0);
  EXPECT_EQ(mem.getDataPerodic(7,0), 1);
  EXPECT_EQ(mem.getDataPerodic(8,0), 2);

  // 負の方向で範囲を超過している
  EXPECT_EQ(mem.getDataPerodic(-1,0), 2);
  EXPECT_EQ(mem.getDataPerodic(-2,0), 1);
  EXPECT_EQ(mem.getDataPerodic(-3,0), 0);

  // 負の方向で範囲を超過している（２週目）
  EXPECT_EQ(mem.getDataPerodic(-4,0), 2);
  EXPECT_EQ(mem.getDataPerodic(-5,0), 1);
  EXPECT_EQ(mem.getDataPerodic(-6,0), 0);
}

// ループ状態でデータをセットできることを確認する
TEST(Memory2d, setDataPerodic_succ001)
{
  Memory2d<int> mem(3,2,0);

  bool isExpected = true;

  for (int v = -30; v < 30; v++)
  {
    for (int u = -30; u < 30; u++)
    {
      int setGetVal = u + u * 60;
      mem.setDataPerodic(u,v, setGetVal);
      if (mem.getDataPerodic(u,v) != setGetVal)
      {
        isExpected = false;
      }
    }
  }
  EXPECT_EQ(isExpected, true);
}

// Memory2d に別の Memory2d データを上書きで書き込めることを確認する
// [0,1,2]
// [3,4,5] に
// [100,101,102]
// [103,104,105] を上書きする
TEST(Memory2d, writeMemory2dWithTrimOutOfRange_succ001)
{
  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u,v, u + v * 3 + 100);
    }
  }

  memBase.writeMemory2dWithTrimOutOfRange(memUpdate, 0, 0, 3,2, 0,0);

  // 1行目
  EXPECT_EQ(memBase.getDataPerodic(0,0), 100);
  EXPECT_EQ(memBase.getDataPerodic(1,0), 101);
  EXPECT_EQ(memBase.getDataPerodic(2,0), 102);

  // 2行目
  EXPECT_EQ(memBase.getDataPerodic(0, 1), 103);
  EXPECT_EQ(memBase.getDataPerodic(1, 1), 104);
  EXPECT_EQ(memBase.getDataPerodic(2, 1), 105);
}

// Memory2d に別の Memory2d データを上書きで書き込めることを確認する
// [0,1,2]
// [3,4,5]
//  の要素の 座標(1,1) から
// [100,101,102]
// [103,104,104]
// のデータで上書きする。結果、以下を期待する。
// [  0,  1,  2]
// [  3,100,101]
TEST(Memory2d, writeMemory2dWithTrimOutOfRange_succ002)
{
  Memory2d<int> memBase(3,2, 0);
  Memory2d<int> memUpdate(3,2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u, v, u + v * 3 + 100);
    }
  }

  memBase.writeMemory2dWithTrimOutOfRange(memUpdate, 0, 0, 3, 2, 1, 1);

  // 1行目
  EXPECT_EQ(memBase.getDataPerodic(0, 0), 0);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 1);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 2);

  // 2行目
  EXPECT_EQ(memBase.getDataPerodic(0, 1), 3);
  EXPECT_EQ(memBase.getDataPerodic(1, 1), 100); // 上書き
  EXPECT_EQ(memBase.getDataPerodic(2, 1), 101); // 上書き
}

// Memory2d に別の Memory2d データを上書きで書き込めることを確認する
// [0,1,2]
// [3,4,5]
//  の要素の 座標(1,1) から 幅 (1,1) で
// [100,101,102]
// [103,104,104]
// のデータを使って上書きする。結果、以下を期待する。
// [  0,  1,  2]
// [  3,100,  5]
TEST(Memory2d, writeMemory2dWithTrimOutOfRange_succ003)
{
  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u, v, u + v * 3 + 100);
    }
  }

  memBase.writeMemory2dWithTrimOutOfRange(memUpdate, 0, 0, 1, 1, 1, 1);

  // 1行目
  EXPECT_EQ(memBase.getDataPerodic(0, 0), 0);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 1);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 2);

  // 2行目
  EXPECT_EQ(memBase.getDataPerodic(0, 1), 3);
  EXPECT_EQ(memBase.getDataPerodic(1, 1), 100); // 上書き
  EXPECT_EQ(memBase.getDataPerodic(2, 1), 5);
}

// データ 100 はマスクする指定をし、[0,1,2] に [100,555,100] をコピー指定する
// 結果、[0,555,2] を期待
TEST(Memory2d, writeMemory2dWithTrimOutOfRange_succ005)
{
  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u, v, u + v * 3 + 100);
    }
  }


  // マスク機能を ON にし、100 をマスク対象に指定
  memBase.setEnableMaskCondition(100);
  memBase.writeMemory2dWithTrimOutOfRange(memUpdate, 0, 0, 3, 2, 0, 0);

  EXPECT_EQ(memBase.getDataPerodic(0, 0), 0);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 101);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 102);

  // マスク機能を OFF にし、再度コピー
  memBase.setDisableMaskCondition();
  memBase.writeMemory2dWithTrimOutOfRange(memUpdate, 0, 0, 3, 2, 0, 0);

  EXPECT_EQ(memBase.getDataPerodic(0, 0), 100);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 101);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 102);
}

// Memory2d に別の Memory2d データを上書きで書き込めることを確認する
TEST(Memory2d, writeMemory2dWithPerodicOutOfRange_succ001)
{

  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u, v, u + v * 3 + 100);
    }
  }

  memBase.writeMemory2dWithPerodicOutOfRange(memUpdate, 0, 0, 3, 2, 0, 0);

  // 1行目
  EXPECT_EQ(memBase.getDataPerodic(0, 0), 100);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 101);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 102);

  // 2行目
  EXPECT_EQ(memBase.getDataPerodic(0, 1), 103);
  EXPECT_EQ(memBase.getDataPerodic(1, 1), 104);
  EXPECT_EQ(memBase.getDataPerodic(2, 1), 105);
}

// Memory2d に別の Memory2d データを上書きで書き込めることを確認する
TEST(Memory2d, writeMemory2dWithPerodicOutOfRange_succ002)
{
  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
      memUpdate.setWithIgnoreOutOfRangeData(u, v, u + v * 3 + 100);
    }
  }

  memBase.writeMemory2dWithPerodicOutOfRange(memUpdate, 0, 0, 3, 2, 1, 1);

  // 1行目
  EXPECT_EQ(memBase.getDataPerodic(0, 0), 105);
  EXPECT_EQ(memBase.getDataPerodic(1, 0), 103);
  EXPECT_EQ(memBase.getDataPerodic(2, 0), 104);

  // 2行目
  EXPECT_EQ(memBase.getDataPerodic(0, 1), 102);
  EXPECT_EQ(memBase.getDataPerodic(1, 1), 100);
  EXPECT_EQ(memBase.getDataPerodic(2, 1), 101);
}


// メモリ情報の一部をコピーした別メモリを取得する
// [0,1,2]
// [3,4,5]
// [6,7,8] 
// から
// [4,5]
// [7,8] を取得する
TEST(Memory2d, getCopyRange_succ001)
{
  Memory2d<int> memBase(3, 3, 0);

  for (int v = 0; v < 3; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
    }
  }
  std::shared_ptr<Memory2d<int>> copyMem = memBase.getCopyRange(1, 1, 2, 2);

  EXPECT_EQ(copyMem->size(), 2*2);
  EXPECT_EQ(copyMem->getDataPerodic(0, 0), 4);
  EXPECT_EQ(copyMem->getDataPerodic(1, 0), 5);
  EXPECT_EQ(copyMem->getDataPerodic(0, 1), 7);
  EXPECT_EQ(copyMem->getDataPerodic(1, 1), 8);
}

/// CRC を計算する
TEST(Memory2d, calcCrc32_succ001)
{
  Memory2d<int> memBase(5, 1, 0);

  for (int i = 0; i < 5; i++)
  {
    memBase.setWithIgnoreOutOfRangeData(i,0, i);
  }
  EXPECT_EQ(memBase.calcCrc32(), 1282081847);
}

/// CRC を計算する
TEST(Memory2d, calcCrc32_succ002)
{
  Memory2d<int> memBase(3, 2, 0);
  Memory2d<int> memUpdate(3, 2, 0);

  for (int v = 0; v < 2; v++)
  {
    for (int u = 0; u < 3; u++)
    {
      memBase.setWithIgnoreOutOfRangeData(u, v, u + v * 3);
    }
  }
  EXPECT_EQ(memBase.calcCrc32(), 2232219709);
}