#include "../../pch.h"

using namespace nl;

// 各種ログレベルが正しく出力されること
TEST(Logger, case1)
{
  logger.info << "Here is the log before limited with str aaa" << std::endl;
  logger.doFilter("aaa");
  logger.info << "this is a test INFO log!aaabbbccc" << std::endl;
  logger.error << "this is a test ERROR logddddd!aaagggg" << std::endl;
  logger.debug << "this is a filter test aaa desu!!" << std::endl;
}
