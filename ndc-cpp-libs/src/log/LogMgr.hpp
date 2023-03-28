#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "LogStream.hpp"
#include "LOGLEVEL.hpp"

namespace nl
{

  class LogMgr
  {
  public:
    LogStream debug;
    LogStream info;
    LogStream warn;
    LogStream error;

  public:
    LogMgr();

    ~LogMgr()
    {
    }
    void setLogAppender(ILogAppender &appender);
    void doFilter(const std::string &substr);
    void setLoglevel(const LOGLEVEL level);
  };
}
