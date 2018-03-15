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

#ifndef EEPROM_SIZE
#error EEPROM size not defined
#endif

#ifndef DBMS_MAX_BLOCKS
#error Number of DBMS blocks undefined
#endif

#ifndef DBMS_MAX_SECTIONS
#error Number of DBMS sections undefined
#endif

#ifndef LOW_BYTE
#define LOW_BYTE(value)                  ((value) & 0xFF)
#endif

#ifndef HIGH_BYTE
#define HIGH_BYTE(value)                 (((value) >> 8) & 0xFF)
#endif

#ifndef BIT_READ
#define BIT_READ(value, bit)             (((value) >> (bit)) & 0x01)
#endif

#ifndef BIT_WRITE
#define BIT_SET(value, bit)              ((value) |= (1UL << (bit)))
#define BIT_CLEAR(value, bit)            ((value) &= ~(1UL << (bit)))
#define BIT_WRITE(value, bit, bitvalue)  (bitvalue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))
#endif

///
/// \ingroup avrDBMS
/// @{

class DBMS
{
    public:
    DBMS();
    static void init();
    static void clear();
    static bool read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t &value);
    static int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    static bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue);
    static uint32_t getDBsize();
    static bool addBlocks(uint8_t numberOfBlocks);
    static bool addSection(uint8_t blockID, dbSection_t section);
    static void initData(initType_t type = initFull);
    static void setHandleRead(bool(*fptr)(uint32_t address, sectionParameterType_t type, int32_t &value));
    static void setHandleWrite(bool(*fptr)(uint32_t address, int32_t value, sectionParameterType_t type));

    private:
    static bool checkParameters(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    static void commitLayout();
};

/// @}