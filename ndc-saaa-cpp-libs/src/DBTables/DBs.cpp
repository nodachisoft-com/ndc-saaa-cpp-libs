#include "DBs.hpp"

using namespace nl;
using namespace a3c;
using namespace std::string_literals;

DBs::DBs()
{
  initialized = false;
  dbdataPath = ".\\..\\..\\ndc-saaa-cpp-libs\\resources\\dbdata\\";
}

bool DBs::init()
{
  if (initialized)
  {
    // 初期化済み
    logger.info << "DBs::init() already initialized." << std::endl;
    return false;
  }

  {
    // LABELS_GEO データ読み込み
    std::string path(dbdataPath + "labels_geo.csv");
    FileAccessor fa(path);
    if (fa.readFileSync() == false)
    {
      logger.error << "DBs::init() Could'nt read labels_geo.csv" << std::endl;
    }
    std::string csvData = fa.getMemoryBank()->readStringToEnd();
    CSVReader csvReader(',', '\\', '#');
    csvReader.readCsv(csvData);
    int count = TABLE_LABELS_GEO.readCSV(csvReader);
    logger.info << "DBs::init():TABLE_LABELS_GEO csv to DB loaded. count="s << std::to_string(count) << "."s;
  }
  return true;
}
