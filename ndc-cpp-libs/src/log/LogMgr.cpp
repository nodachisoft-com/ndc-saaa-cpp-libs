#include "LogMgr.hpp"

using namespace nl;

LogMgr::LogMgr()
{
  // デフォルト値
  setLoglevel(LOGLEVEL::INFO);
  debug.setLoglevel(LOGLEVEL::DEBUG);
  info.setLoglevel(LOGLEVEL::INFO);
  warn.setLoglevel(LOGLEVEL::WARN);
  error.setLoglevel(LOGLEVEL::ERROR);
}

void LogMgr::setLogAppender(ILogAppender &appender)
{
  debug.setLogAppender(appender);
  info.setLogAppender(appender);
  warn.setLogAppender(appender);
  error.setLogAppender(appender);
}

void LogMgr::doFilter(const std::string &substr)
{
  debug.doFilter(substr);
  info.doFilter(substr);
  warn.doFilter(substr);
  error.doFilter(substr);
}
void LogMgr::setLoglevel(const LOGLEVEL level)
{
  // 全てのログレベルの値を初期化
  debug.setEnable(false);
  info.setEnable(false);
  warn.setEnable(false);
  error.setEnable(true);

  // 場合分け
  if (level == LOGLEVEL::ERROR)
  {
    return;
  }
  warn.setEnable(true);
  if (level == LOGLEVEL::WARN)
  {
    return;
  }
  info.setEnable(true);
  if (level == LOGLEVEL::INFO)
  {
    return;
  }
  debug.setEnable(true);
}
