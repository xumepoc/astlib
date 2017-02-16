///
/// \package astlib
/// \file ValueDecoder.h
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date 7Feb.,2017 
/// \brief definicia typu
///
/// (C) Copyright 2017 R-SYS s.r.o
/// All rights reserved.
///


#pragma once

#include "astlib/model/BitsDescription.h"
#include "astlib/model/ItemDescription.h"

namespace astlib
{

/**
 * Base interface for dekoder callback mechanism.
 * The methods are called on:
 * - begin of record
 * - start of each Uap Data Item
 * - start of each round for repetitive items
 * - each decoded primite value
 * - end of record
 */
class ValueDecoder
{
public:
    struct Context
    {
        Context(const ItemDescription& uapItem, const BitsDescription& bits, int depth) :
            uapItem(uapItem),
            bits(bits),
            depth(depth),
            width(bits.effectiveBitsWidth())
        {
        }

        const ItemDescription& uapItem;
        const BitsDescription& bits;
        int depth = 0;
        int width = 1;
    };

    virtual ~ValueDecoder() = default;

    virtual void begin() = 0;
    virtual void dataItem(const ItemDescription& uapItem) = 0;
    virtual void beginRepetitive(int size) = 0;
    virtual void repetitiveItem(int index) = 0;
    virtual void endRepetitive() = 0;
    virtual void decode(const Context& ctx, Poco::UInt64 value) = 0;
    virtual void end() = 0;
};

/**
 * Wrapper for easy value type handling.
 * Implements decode() method, converts value to apropriate type and call its typed callback.
 */
class TypedValueDecoder :
    public ValueDecoder
{
public:
    virtual void decode(const Context& ctx, Poco::UInt64 value);

    virtual void decodeBoolean(const Context& ctx, bool value) = 0;
    virtual void decodeSigned(const Context& ctx, Poco::Int64 value) = 0;
    virtual void decodeUnsigned(const Context& ctx, Poco::UInt64 value) = 0;
    virtual void decodeReal(const Context& ctx, double value) = 0;
    virtual void decodeString(const Context& ctx, const std::string& value) = 0;
};

}



