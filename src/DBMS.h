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
    DBMS();
    static bool init(dbBlock_t *pointer, uint8_t numberOfBlocks);
    static void clear();
    static bool read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t &value);
    static int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    static bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue);
    static uint32_t getDBsize();
    static void initData(initType_t type = initFull);
    static void setHandleRead(bool(*fptr)(uint32_t address, sectionParameterType_t type, int32_t &value));
    static void setHandleWrite(bool(*fptr)(uint32_t address, int32_t value, sectionParameterType_t type));

    private:
    static bool checkParameters(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
};
