#pragma once
#include <memory>
#include "../exception/index.hpp"

namespace nl
{

  template <class T>
  class Memory3d
  {
    T *buf = NULL;
    T outOfRangeData;

    T initialData;

    /// @brief 他の Memory3d データで上書きする際に、特定のデータを無視するか
    bool isMaskConditionEnable = false;

    /// @brief 無視するデータ定義。isMaskConditionEnable が true なら読み取られる
    T maskConditionData;

    /// @brief データの行方向のサイズ
    int width;

    /// @brief データの列方向のサイズ
    int height;

    /// @brief データの深さ方向のサイズ
    int depth;

  private:
    int calcIndexPos(int x, int y, int z)
    {
      return (z * width * height) + y * width + x;
    }

  public:
    Memory3d(const int _width, const int _height, const int _depth, const T initialValue)
    {
      resizeMemory(_width, _height, _depth);
      setWholeData(initialValue);
    }

    void resizeMemory(const int _width, const int _height, const int _depth)
    {
      if (buf != NULL)
      {
        // 初回の初期化以外では前回確保分のメモリ削除を行う
        delete[] buf;
      }

      try
      {
        buf = new T[_width * _height * _depth];
        width = _width;
        height = _height;
        depth = _depth;
      }
      catch (std::bad_alloc &ex)
      {
        std::string msg("Memory3d: Failed To Allocate Memory!");
        throw OutOfMemoryException(msg);
      }
    }

    void setWholeData(const T initialValue)
    {
      // 初期化
      for (int z = 0; z < depth; z++)
      {
        for (int v = 0; v < height; v++)
        {
          for (int u = 0; u < width; u++)
          {
            buf[calcIndexPos(u, v, z)] = initialValue;
          }
        }
      }
      initialData = initialValue;
    }

    ~Memory3d()
    {
      delete[] buf;
    }

    int size()
    {
      return width * height * depth;
    }

    int getWidth()
    {
      return width;
    }

    int getHeight()
    {
      return height;
    }

    int getDepth()
    {
      return depth;
    }


    /// @brief CRC32 の計算
    /// @return CRC32 の計算結果
    unsigned long calcCrc32()
    {
      size_t objSize = sizeof(T);
      int bufSize = size();
      Crc32 crc;
      for (int index = 0; index < bufSize; index++) {
        unsigned char* byteArray = (unsigned char*)(void*)&buf[index];
        for (int objIndex = 0; objIndex < objSize; objIndex++) {
          crc.calcUpdate(byteArray[objIndex]);
        }
      }
      return crc.getHash();
    }


    void setOutOfRangeData(const T t)
    {
      outOfRangeData = t;
    }

    /// @brief ほかの Memory3d データで上書きする際に、無視するデータを指定
    /// @param t 無視する対象のデータ
    void setMaskConditionWhenWriteMemory(const T t)
    {
      maskConditionData = t;
    }

    void setEnableMaskCondition(const T mask)
    {
      isMaskConditionEnable = true;
      maskConditionData = mask;
    }

    void setDisableMaskCondition()
    {
      isMaskConditionEnable = false;
    }

    bool setWithIgnoreOutOfRangeData(const int x, const int y, const int z, const T val)
    {
      if ((x < 0 || width <= x) || (y < 0 || height <= y) || (z < 0 || depth <= z))
      {
        return false;
      }

      buf[calcIndexPos(x, y, z)] = val;
      return true;
    }

    T getWithIgnoreOutOfRangeData(const int x, const int y, const int z)
    {
      if ((x < 0 || width <= x) || (y < 0 || height <= y) || (z < 0 || depth <= z))
      {
        return outOfRangeData;
      }
      return buf[calcIndexPos(x, y, z)];
    }

    /// @brief
    ///   位置 x に対してループとなるように要素を返す
    ///   例 データ列 [3,4,5] => 3, 4, 5, 3, 4, 5 ... と結果が返る
    /// @param x 取得する要素の位置
    /// @return 取得した要素
    T getDataPerodic(const int x, const int y, const int z)
    {
      int modX = (x < 0) ? (width - (-x % width)) % width : (x % width);
      int modY = (y < 0) ? (height - (-y % height)) % height : (y % height);
      int modZ = (z < 0) ? (depth - (-z % depth)) % depth : (z % depth);
      return buf[calcIndexPos(modX, modY, modZ)];
    }

    /// @brief
    ///  位置 x に対してループとなるよう要素番号を射影して値をセットする
    /// @param x 設定する要素の位置
    /// @param value 設定する値
    void setDataPerodic(const int x, const int y, const int z, const T value)
    {
      int modX = (x < 0) ? (width - (-x % width)) % width : (x % width);
      int modY = (y < 0) ? (height - (-y % height)) % height : (y % height);
      int modZ = (z < 0) ? (depth - (-z % depth)) % depth : (z % depth);
      buf[calcIndexPos(modX, modY, modZ)] = value;
    }

    void writeMemory3dWithTrimOutOfRange(const Memory3d<T> &src, const int srcBeginX, const int srcBeginY, const int srcBeginZ, const int copyLengthWidth, const int copyLengthHeight, const int copyLengthDepth, const int destBeginX, const int destBeginY, const int destBeginZ)
    {
      if ((srcBeginX < 0 || srcBeginX + copyLengthWidth > src.width) || (srcBeginY < 0 || srcBeginY + copyLengthHeight > src.height) || (srcBeginZ < 0 || srcBeginZ + copyLengthDepth > src.depth))
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }

      for (int z = 0; z < copyLengthDepth && z + destBeginZ < depth; z++)
      {
        for (int v = 0; v < copyLengthHeight && v + destBeginY < height; v++)
        {
          for (int u = 0; u < copyLengthWidth && u + destBeginX < width; u++)
          {
            int srcIndex = calcIndexPos(u + srcBeginX, v + srcBeginY, z + srcBeginZ);
            int destIndex = calcIndexPos(u + destBeginX, v + destBeginY, z + destBeginZ);

            if (isMaskConditionEnable)
            {

              if (maskConditionData == src.buf[srcIndex])
              {
                // データが無視する対象と合致するため、上書き処理を行わない
                continue;
              }
            }
            buf[destIndex] = src.buf[srcIndex];
          }
        }
      }
    }

    void writeMemory3dWithPerodicOutOfRange(const Memory3d<T> &src, const int srcBeginX, const int srcBeginY, const int srcBeginZ, const int copyLengthWidth, const int copyLengthHeight, const int copyLengthDepth, const int destBeginX, const int destBeginY, const int destBeginZ)
    {
      if (srcBeginX < 0 || srcBeginY < 0 || srcBeginZ < 0)
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }
      for (int z = 0; z < copyLengthDepth < depth; z++)
      {
        for (int v = 0; v < copyLengthHeight; v++)
        {
          for (int u = 0; u < copyLengthWidth; u++)
          {
            int srcIndex = calcIndexPos(u + srcBeginX, v + srcBeginY, z + srcBeginZ);
            int destIndex = calcIndexPos((u + destBeginX) % width, (v + destBeginY) % height, (z + destBeginZ) % depth);

            if (isMaskConditionEnable)
            {
              if (maskConditionData == src.buf[srcIndex])
              {
                // データが無視する対象と合致するため、上書き処理を行わない
                continue;
              }
            }
            buf[destIndex] = src.buf[srcIndex];
          }
        }
      }
    }

    /// @brief メモリに含まれる立方体の範囲をコピーして、新しいインスタンスを作成して返す
    /// @param from
    /// @param to
    /// @param copyTo
    std::shared_ptr<Memory3d<T>> getCopyRange(const int fromX, const int fromY, const int fromZ, const int toX, const int toY, const int toZ)
    {
      if ((0 > fromX || width < toX || fromX > toX) || (0 > fromY || height < toY || fromY > toY) || (0 > fromZ || depth < toZ || fromZ > toZ))
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }

      // コピーするサイズ
      int newWidth = toX - fromX + 1;
      int newHeight = toY - fromY + 1;
      int newDepth = toZ - fromZ + 1;

      // コピー先のインスタンスを生成
      std::shared_ptr<Memory3d<T>> newMem(new Memory3d<T>(newWidth, newHeight, newDepth, initialData));
      for (int z = 0; z < newDepth; z++)
      {
        for (int v = 0; v < newHeight; v++)
        {
          for (int u = 0; u < newWidth; u++)
          {
            int index = calcIndexPos(fromX + u, fromY + v, fromZ + z);
            newMem->setWithIgnoreOutOfRangeData(u, v, z, buf[index]);
          }
        }
      }
      return newMem;
    }
  };

};
