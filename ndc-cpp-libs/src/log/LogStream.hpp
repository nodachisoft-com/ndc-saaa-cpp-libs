#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include "LogHistory.hpp"
#include "ILogAppender.hpp"
#include "LogMgr.hpp"
#include "StdOutLogAppender.hpp"
#include "../timewatch/TimeWatch.hpp"

namespace nl
{
  class LogStream : public std::ostream
  {
  private:
    ILogAppender *_appender;
    bool _enable;
    std::ostringstream oss;
    LOGLEVEL logLv;
    bool isFiltered;
    std::regex filterCondition;
    std::vector<LogHistory> history;

    const size_t HistoryMaxSize = 10;

  public:
    LogStream() : std::ostream(std::cout.rdbuf()), _enable(true), logLv(LOGLEVEL::INFO), isFiltered(false), history({})
    {
      _appender = new StdOutLogAppender();
    }
    ~LogStream()
    {
      delete _appender;
    }

    void setLogAppender(ILogAppender &appender)
    {
      _appender = &appender;
    }

    void setEnable(const bool enable)
    {
      _enable = enable;
    }
    bool isEnable()
    {
      return _enable;
    }

    void doFilter(const std::string substr)
    {
      isFiltered = true;
      filterCondition = std::regex(std::string(".*").append(substr).append(".*"));
    }

    void removeFilter()
    {
      isFiltered = false;
    }

    void setLoglevel(LOGLEVEL lv)
    {
      logLv = lv;
    }

    /// @brief フィルタが ON であり、フィルタ対象かを判定する
    /// @param msg フィルタ確認する対象文字列
    /// @return フィルタ対象であれば true、対象外であれば false を返す
    bool isFilterTarget(const std::string &msg)
    {
      return (std::regex_match(msg, filterCondition));
    }

    std::vector<LogHistory> getLogHistory()
    {
      return history;
    }

    template <typename T>
    LogStream &operator<<(const T &val)
    {
      if (_enable == false)
      {
        return *this;
      }
      oss << val;
      return *this;
    }
    // std::endl
    LogStream &operator<<(std::ostream &(*os)(std::ostream &))
    {
      if (_enable == false)
      {
        return *this;
      }

      // フィルタ機能は有効であるか判定
      if (isFiltered)
      {

        // フィルタ条件に合致しない場合は出力しない
        if (!isFilterTarget(oss.str()))
        {
          oss.str("");
          oss.clear(std::stringstream::goodbit);
          return *this;
        }
      }
      LogHistory logHistory;
      logHistory.timestamp = TimeWatch::getNowMsAsStr_hhmmssSSS();
      logHistory.logLevel = logLv;
      logHistory.logMessage = oss.str();
      _appender->outputLog(logHistory);

      // History 追加
      history.push_back(logHistory);
      if (history.size() > HistoryMaxSize)
      {
        history.erase(history.begin());
      }

      oss.str("");
      oss.clear(std::stringstream::goodbit);
      return *this;
    }
  };
}
