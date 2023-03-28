#pragma once
#include "LogMgr.hpp"

namespace nl
{
  class ILogAppender
  {
  public:
    virtual void outputLog(const LogHistory &log) = 0;
    virtual ~ILogAppender()
    {
    }
  };
}
