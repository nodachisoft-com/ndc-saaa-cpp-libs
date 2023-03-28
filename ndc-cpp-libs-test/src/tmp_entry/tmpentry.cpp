#include "../../src/ndclibs.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace nl;

int main()
{
  nl::logger.debug << "DEBUG Message!" << std::endl;
  nl::logger.info << "INFO Message!" << std::endl;
  nl::logger.warn << "WARN Message!" << std::endl;
  nl::logger.error << "ERROR Message!" << std::endl;

  nl::logger.doFilter("123");

  nl::logger.info << "sss123bbbb" << std::endl;
  nl::logger.info << "sssbbbb" << std::endl;
  nl::logger.info << "123sbbbb" << std::endl;
  nl::logger.info << "sbbbb123" << std::endl;
  nl::logger.info << "11999OKKK" << std::endl;

  std::vector<LogHistory> history = nl::logger.info.getLogHistory();
  for (LogHistory elem : history)
  {
    std::cout << "HIST:" << elem.timestamp << " : " << elem.logMessage << std::endl;
  }
}
