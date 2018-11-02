/*
    Copyright 2017-2018 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "DataTypes.h"

#ifndef LESSDB_SIZE
#error Database size not defined
#endif

class DBMS
{
    public:
    DBMS(bool (*readCallback)(uint32_t address, sectionParameterType_t type, int32_t &value), bool (*writeCallback)(uint32_t address, int32_t value, sectionParameterType_t type));
    bool setLayout(dbBlock_t *pointer, uint8_t numberOfBlocks);
    void clear();
    bool read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t &value);
    int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue);
    uint32_t getDBsize();
    void initData(initType_t type = initFull);

    private:
    bool checkParameters(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    uint16_t getSectionAddress(uint8_t blockID, uint8_t sectionID);

    ///
    /// \brief Holds amount of blocks.
    ///
    uint8_t blockCounter;

    ///
    /// \brief Holds total memory usage for current database layout.
    ///
    uint32_t memoryUsage;

    ///
    /// \brief Cached values for bit and half-byte parameters.
    /// Used if current requested address is the same as previous one.
    /// @{

    uint8_t lastValue = 0;
    uint16_t lastAddress = LESSDB_SIZE;

    /// @}

    ///
    /// \brief Pointer to array of DBMS blocks.
    ///
    dbBlock_t *block = nullptr;

    ///
    /// \brief Function pointer used to read the memory contents.
    ///
    bool (*readCallback)(uint32_t address, sectionParameterType_t type, int32_t &value);

    ///
    /// \brief Function pointer used to update the memory contents.
    ///
    bool (*writeCallback)(uint32_t address, int32_t value, sectionParameterType_t type);
};