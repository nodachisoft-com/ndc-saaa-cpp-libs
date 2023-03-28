#include "CSVReader.hpp"

using namespace nl;

void CSVReader::readCsv(const std::string &csv_data)
{
  std::vector<std::vector<std::string>> result;
  std::istringstream stream(csv_data);
  std::string line; // 読み込み行
  while (std::getline(stream, line))
  {
    if (line[0] == comment_char_)
    {
      // コメントする対象である
      continue;
    }
    std::vector<std::string> row;
    size_t start = 0, index = 0;
    bool escape = false;
    while (index < line.size())
    {
      if (line[index] == escape_char_)
      {
        escape = true;
      }
      else if (line[index] == delimiter_ && !escape)
      {
        row.push_back(line.substr(start, index - start));
        start = index + 1;
      }
      else
      {
        escape = false;
      }
      index++;
    }
    row.push_back(line.substr(start));
    result.push_back(row);
  }
  data = result;
}

std::string CSVReader::getCell(int col, int row)
{
  size_t rowSize = data.size();
  if (row < 0 || rowSize < (size_t)row)
  {
    std::string msg;
    msg += "CSVReader::getCell(" + std::to_string(row) + "," + std::to_string(col) + ") is out of range.";
    throw OutOfRangeAccessException(msg);
  }
  size_t colSize = data[row].size();
  if (col < 0 || colSize < (size_t)col)
  {
    std::string msg;
    msg += "CSVReader::getCell(" + std::to_string(row) + "," + std::to_string(col) + ") is out of range.";
    throw OutOfRangeAccessException(msg);
  }
  return data[row][col];
}

std::vector<std::string> CSVReader::getRowCellList(int rowIndex)
{

  size_t rowSize = data.size();
  if (rowIndex < 0 || rowSize < (size_t)rowIndex)
  {
    std::string msg;
    msg += "CSVReader::getRowCellList(" + std::to_string(rowIndex) + ") is out of range.";
    throw OutOfRangeAccessException(msg);
  }
  return data[rowIndex];
}
