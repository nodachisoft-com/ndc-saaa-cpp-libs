#include "../../pch.h"

#include <string>

using namespace nl;

// カレントディレクトリのパスを取得する
TEST(FileAccessMgr, curDir_case1)
{
  std::string curDir(FileAccessMgr::getCurDir());
  size_t length = curDir.length();
  // 1 以上の文字列が取得できていること

  EXPECT_GT(length, 1);
}

// ディレクトリ作成、情報取得、削除を行う
TEST(FileAccessMgr, directory_operation_case1)
{
  std::string targetDir = TESTTMP_DIR + "makedir_case1";

  // 対象ディレクトリがまだ存在しないこと
  FileType dircheck1 = FileAccessMgr::isExistFileOrDir(targetDir);
  EXPECT_EQ(dircheck1, FileType::FILE_NOT_FOUND);

  // makedir が成功していること
  EXPECT_EQ(FileAccessMgr::makedir(targetDir), 1);

  // path の情報を取得し ディレクトリであること内容に問題ないこと
  FileType newDirAcc = FileAccessMgr::isExistFileOrDir(targetDir);
  EXPECT_EQ(newDirAcc, FileType::DIR);

  // removedir が成功していること
  EXPECT_EQ(FileAccessMgr::removedir(targetDir), 1);

  // 対象ディレクトリが削除されており、存在しないこと
  FileType dircheck2 = FileAccessMgr::isExistFileOrDir(targetDir);
  EXPECT_EQ(dircheck2, FileType::FILE_NOT_FOUND);
}

// 深いディレクトリを作成し、再帰的にデータを取得した後、ディレクトリを削除する
TEST(FileAccessMgr, getDirsRecursively_case1)
{
  std::vector<FileAccessor> dirs = FileAccessMgr::getDirsRecursively(TESTTMP_DIR);
  EXPECT_EQ(dirs.size(), 0); // 0 件のディレクトリ

  // 作成するディレクトリの一覧（上から順番に作成）
  std::vector<std::string> dirPathList = {
      TESTTMP_DIR + "makedir_case2",
      TESTTMP_DIR + "makedir_case2\\001",
      TESTTMP_DIR + "makedir_case2\\002",
      TESTTMP_DIR + "makedir_case2\\003",
      TESTTMP_DIR + "makedir_case2\\004",
      TESTTMP_DIR + "makedir_case2\\004\\ab",
      TESTTMP_DIR + "makedir_case2\\004\\cde"};

  // ディレクトリの作成
  for (size_t i = 0; i < dirPathList.size(); i++)
  {
    EXPECT_EQ(FileAccessMgr::makedir(dirPathList[i]), true);
  }

  // 一覧取得
  dirs = FileAccessMgr::getDirsRecursively(TESTTMP_DIR);
  EXPECT_EQ(dirs.size(), 7); // 7 件のディレクトリが取得できている
  for (size_t i = 0; i < dirs.size(); i++)
  {
    bool isExist = false;
    for (size_t m = 0; m < dirPathList.size(); m++)
    {
      if (dirs[i].getFilePath() == dirPathList[m])
      {
        isExist = true;
      }
    }
    EXPECT_EQ(isExist, true);
  }

  // ディレクトリの削除(深いほうからソートして順番に削除)
  std::vector<std::string> depthOrderPathList = util::sortStringVectorByLengthDesc(dirPathList);
  //EXPECT_EQ(depthOrderPathList[0], TESTTMP_DIR + "makedir_case2\\004\\cde");
  for (size_t i = 0; i < depthOrderPathList.size(); i++)
  {
    EXPECT_EQ(FileAccessMgr::removedir(depthOrderPathList[i]), true);
  }
  // 一覧取得
  dirs = FileAccessMgr::getDirsRecursively(TESTTMP_DIR);
  EXPECT_EQ(dirs.size(), 0); // 0 件のディレクトリ

}
