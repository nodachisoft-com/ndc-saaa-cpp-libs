#pragma once
#include <string>
#include "LOGLEVEL.hpp"

namespace nl
{
  struct LogHistory
  {
    std::string timestamp;
    LOGLEVEL logLevel;
    std::string logMessage;
    LogHistory() : timestamp(""), logLevel(LOGLEVEL::INFO), logMessage("") {

    }
  };
}
