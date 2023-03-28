#include "../../pch.h"


#include <string>
using namespace nl;
// 存在しないファイルへのアクセス処理
TEST(FileAccessor, fa_constructor_case1)
{
  std::string path = TESTTMP_DIR + "fa_case1_not_exists.txt";
  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }


  FileAccessor fa(path);
  EXPECT_EQ(fa.calcMemoryCrc32(), 0);
  EXPECT_EQ(fa.readFileSync(), false);
  EXPECT_EQ(fa.getFilesize(), 0);
  EXPECT_EQ(fa.getFiletype(), FileType::FILE_NOT_FOUND);
  EXPECT_EQ(fa.getMemoryBank()->getUsingSize(), 0);
  EXPECT_EQ(fa.getFilePath(), path);
  EXPECT_EQ(fa.isOpenFile(), false);
  EXPECT_EQ(fa.getProgress(), 0.0f);
  EXPECT_EQ(fa.getFileStatus(), FileStatus::NOT_LOADING);
}

// 存在しないファイルへのアクセス処理でファイルを作成
TEST(FileAccessor, fa_case2_writeFileSync)
{
  std::string path = TESTTMP_DIR + "fa_case2_newfile.txt";
  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }

  FileAccessor fa(path);
  EXPECT_EQ(fa.getFiletype(), FileType::FILE_NOT_FOUND);
  EXPECT_EQ(fa.getMemoryBank()->getUsingSize(), 0);
  EXPECT_EQ(fa.getFileStatus(), FileStatus::NOT_LOADING);

  // メモリバッファに文字列追加
  std::string msg("This is Test Message!");
  for (int i = 0; i < msg.size(); i++)
  {
    fa.getMemoryBank()->appendByte(msg[i]);
  }
  EXPECT_EQ(fa.writeFileSync(), true); // 書き込み

  // 書き込みが成功した場合、ファイルへのアクセスが可能である
  EXPECT_EQ(fa.getFiletype(), FileType::FILE);
  EXPECT_EQ(fa.getFileStatus(), FileStatus::AVAILABLE);
}

// 1.存在しないファイルへのアクセス処理でファイルを作成
// 2.作成したファイルを読み込み、データ取り出し
TEST(FileAccessor, fa_case2_writeFileSync_and_read)
{
  std::string path = TESTTMP_DIR + "fa_case2_writeFileSync_and_read.txt";
  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }


  FileAccessor fa(path);
  EXPECT_EQ(fa.getFiletype(), FileType::FILE_NOT_FOUND);

  // メモリバッファに文字列追加
  std::string msg1("1st Message!");
  std::string msg2("2nd Next Message!");
  fa.getMemoryBank()->appendStringWithLength(msg1);
  fa.getMemoryBank()->appendStringWithLength(msg2);

  EXPECT_EQ(fa.writeFileSync(), true); // 書き込み

  // 別途新しくファイルを開く
  FileAccessor fa2(path);
  EXPECT_EQ(fa2.getFiletype(), FileType::FILE);
  EXPECT_EQ(fa2.readFileSync(), true);                                         // ファイル内容の読み込み
  EXPECT_EQ(fa2.getMemoryBank()->readStringWithLength(), "1st Message!");      // 取り出し
  EXPECT_EQ(fa2.getMemoryBank()->readStringWithLength(), "2nd Next Message!"); // 取り出し

}

// 追記モードでファイルに文字列を書き込み
TEST(FileAccessor, fa_case3_appendStringToFileSync)
{
  std::string path = TESTTMP_DIR + "fa_case3_appendStringToFileSync.txt";

  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }


  EXPECT_EQ(FileAccessor::appendStringToFileSync(path, "abcd"), true);
  FileAccessor fa(path);
  EXPECT_EQ(fa.readFileSync(), true);
  EXPECT_EQ(0xed82cd11, fa.calcMemoryCrc32());
}

// 大き目のファイルで CRC32 計算を行う
// FileAccessor に渡すメモリの CRC32、ファイル経由での CRC32 、再読み込み後の CRC32 を比較
TEST(FileAccessor, fa_crc32_largefile_case001)
{
  std::string path = TESTTMP_DIR + "fa_crc32_largefile_case001.bin";
  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }


  FileAccessor faWrite(path);
  MemoryBank mem;
  for (int i = 0; i < 1024 * 16; i++)
  {
    mem.appendByte((unsigned char)(i % 255));
  }

  faWrite.setMemoryBank(&mem);
  EXPECT_EQ(333031102, mem.calcCrc32());
  EXPECT_EQ(333031102, faWrite.calcMemoryCrc32());

  faWrite.writeFileSync();

  // 改めてファイルを読み込み
  FileAccessor faRead(path);
  faRead.readFileSync();
  EXPECT_EQ(333031102, faRead.calcMemoryCrc32());
}

// 追記モードで文字列をファイルへ書き出す
TEST(FileAccessor, fa_appendStringToFileSync_case001)
{
  std::string path(TESTTMP_DIR + "fa_appendStringToFileSync_case001.txt");

  // テスト前に対象ファイルがあれば削除
  FileAccessor preCheckFa(path);
  if (preCheckFa.getFiletype() == FileType::FILE) {
      FileAccessMgr::removefile(path);
  }

  std::string msg("This is test!");
  FileAccessor::appendStringToFileSync(path, msg);
  FileAccessor faCheck(path);
  EXPECT_EQ(faCheck.readFileSync(), true);
  EXPECT_EQ(faCheck.calcMemoryCrc32(), 413162141);
}
