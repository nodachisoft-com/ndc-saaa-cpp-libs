#pragma once
#include <memory>
#include "../exception/index.hpp"

namespace nl
{
  template <class T>
  class Memory2d
  {
  private:
    bool isInit;
    T *buf = nullptr;
    T outOfRangeData;

    T initialData;

    /// @brief 他の Memory2d データで上書きする際に、特定のデータを無視するか
    bool isMaskConditionEnable = false;

    /// @brief 無視するデータ定義。isMaskConditionEnable が true なら読み取られる
    T maskConditionData;

    /// @brief データの行方向のサイズ
    int width;

    /// @brief データの列方向のサイズ
    int height;

  private:
    int calcIndexPos(int x, int y)
    {
      return y * width + x;
    }

  public:
    Memory2d()
        : isInit(false)
    {
    }

    Memory2d(const int _width, const int _height, const T initialValue)
        : isInit(false)
    {
      init(_width, _height, initialValue);
    }
    bool init(const int _width, const int _height, const T initialValue)
    {
      resizeMemory(_width, _height);
      setWholeData(initialValue);
      isInit = true;
      return true;
    }

    void resizeMemory(const int _width, const int _height)
    {
      if (buf != nullptr)
      {
        // 初回の初期化以外では前回確保分のメモリ削除を行う
        delete[] buf;
      }

      try
      {
        buf = new T[_width * _height];
        width = _width;
        height = _height;
      }
      catch (std::bad_alloc)// &ex)
      {
        std::string msg("Memory2d: Failed To Allocate Memory!");
        throw OutOfMemoryException(msg);
      }
    }

    void setWholeData(const T initialValue)
    {
      // 初期化
      for (int v = 0; v < height; v++)
      {
        for (int u = 0; u < width; u++)
        {
          buf[calcIndexPos(u, v)] = initialValue;
        }
      }
      initialData = initialValue;
    }

    ~Memory2d()
    {
      delete[] buf;
    }

    int size()
    {
      return width * height;
    }

    int getWidth()
    {
      return width;
    }

    int getHeight()
    {
      return height;
    }

    void setOutOfRangeData(const T t)
    {
      outOfRangeData = t;
    }

    /// @brief ほかの Memory2d データで上書きする際に、無視するデータを指定
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

    bool setWithIgnoreOutOfRangeData(const int x, const int y, const T val)
    {
      if ((x < 0 || width <= x) || (y < 0 || height <= y))
      {
        return false;
      }

      buf[calcIndexPos(x, y)] = val;
      return true;
    }

    T getWithIgnoreOutOfRangeData(const int x, const int y)
    {
      if ((x < 0 || width <= x) || (y < 0 || height <= y))
      {
        return outOfRangeData;
      }
      return buf[calcIndexPos(x, y)];
    }

    /// @brief
    ///   位置 x に対してループとなるように要素を返す
    ///   例 データ列 [3,4,5] => 3, 4, 5, 3, 4, 5 ... と結果が返る
    /// @param x 取得する要素の位置
    /// @return 取得した要素
    T getDataPerodic(const int x, const int y)
    {
      int modX = (x < 0) ? (width - (-x % width)) % width : (x % width);
      int modY = (y < 0) ? (height - (-y % height)) % height : (y % height);

      return buf[calcIndexPos(modX, modY)];
    }

    /// @brief
    ///  位置 x に対してループとなるよう要素番号を射影して値をセットする
    /// @param x 設定する要素の位置
    /// @param value 設定する値
    void setDataPerodic(const int x, const int y, const T value)
    {
      int modX = (x < 0) ? (width - (-x % width)) % width : (x % width);
      int modY = (y < 0) ? (height - (-y % height)) % height : (y % height);
      buf[calcIndexPos(modX, modY)] = value;
    }

    void writeMemory2dWithTrimOutOfRange(const Memory2d<T> &src, const int srcBeginX, const int srcBeginY, const int copyLengthWidth, const int copyLengthHeight, const int destBeginX, const int destBeginY)
    {
      if ((srcBeginX < 0 || srcBeginX + copyLengthWidth > src.width) || (srcBeginY < 0 || srcBeginY + copyLengthHeight > src.height))
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }

      for (int v = 0; v < copyLengthHeight && v + destBeginY < height; v++)
      {
        for (int u = 0; u < copyLengthWidth && u + destBeginX < width; u++)
        {
          int srcIndex = calcIndexPos(u + srcBeginX, v + srcBeginY);
          int destIndex = calcIndexPos(u + destBeginX, v + destBeginY);

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

    void writeMemory2dWithPerodicOutOfRange(const Memory2d<T> &src, const int srcBeginX, const int srcBeginY, const int copyLengthWidth, const int copyLengthHeight, const int destBeginX, const int destBeginY)
    {
      if (srcBeginX < 0 || srcBeginY < 0)
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }
      for (int v = 0; v < copyLengthHeight; v++)
      {
        for (int u = 0; u < copyLengthWidth; u++)
        {
          int srcIndex = calcIndexPos(u + srcBeginX, v + srcBeginY);
          int destIndex = calcIndexPos((u + destBeginX) % width, (v + destBeginY) % height);

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

    /// @brief TODO: copyTo で引数内容を直接編集するのではなく、
    ///   スマートポインタで new した結果を返すのが望ましい。
    /// @param from
    /// @param to
    /// @param copyTo
    std::shared_ptr<Memory2d<T>> getCopyRange(const int fromX, const int fromY, const int toX, const int toY)
    {
      if ((0 > fromX || width <= toX || fromX > toX) || (0 > fromY || height <= toY || fromY > toY))
      {
        // コピー元が範囲外である
        std::string msg(typeid(*this).name());
        msg += ":out of range.";
        throw ArgumentValidatioinException(msg);
      }

      // コピーするサイズ
      int newWidth = toX - fromX;
      int newHeight = toY - fromY;

      // コピー先のインスタンスを生成
      std::shared_ptr<Memory2d<T>> newMem(new Memory2d<T>(newWidth, newHeight, initialData));
      for (int v = 0; v < newHeight; v++)
        for (int u = 0; u < newWidth; u++)
        {
          int index = calcIndexPos(fromX + u, fromY + v);
          newMem->setWithIgnoreOutOfRangeData(u, v, buf[index]);
        }
      return newMem;
    }
  };
};
