#pragma once
#include "../ndclibs.hpp"
#include "SerializeBuffer.hpp"

namespace nl
{

    /**
     * Serialize / Deserialize interface
     */
    class ISerializable
    {
    public:
        /**
         * シリアライズ処理を実現する
         * @param data シリアライズデータとして append する先
         * @return シリアライズ結果
         */
        virtual SerializeBuffer *serialize(SerializeBuffer *buffer);

        /**
         * デシリアライズ処理を実現する
         */
        virtual void deserialize(SerializeBuffer *buffer) = 0;

        virtual ~ISerializable(){};
    };
};
