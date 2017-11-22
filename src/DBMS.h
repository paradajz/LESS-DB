/*
    Copyright 2017 Igor Petroviæ

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

#ifdef DBMS_ENABLE_ASYNC_UPDATE
#ifndef DBMS_UPDATE_BUFFER_SIZE
#error DBMS update buffer size undefined
#endif
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
    void init();
    #if defined(DBMS_ENABLE_ASYNC_UPDATE) || defined(__DOXYGEN__)
    bool checkQueue();
    #endif
    void clear();
    int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex = 0);
    bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue, bool async = false);
    uint32_t getDBsize();

    protected:
    bool addBlock();
    bool addBlocks(uint8_t numberOfBlocks);
    bool addSection(uint8_t blockID, dbSection_t section);
    void commitLayout();
    void initData(initType_t type = initWipe);

    private:
    #if defined(DBMS_ENABLE_ASYNC_UPDATE) || defined(__DOXYGEN__)
    void queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType);
    #endif

    #if defined(DBMS_ENABLE_ASYNC_UPDATE) || defined(__DOXYGEN__)
    ///
    /// \brief Variables used for internal buffer implementation used for async EEPROM update.
    /// @{

    uint8_t     eeprom_update_bufer_param_type[DBMS_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_value[DBMS_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_address[DBMS_UPDATE_BUFFER_SIZE];
    uint8_t     eeprom_update_buffer_head;
    uint8_t     eeprom_update_buffer_tail;

    /// @}
    #endif

    ///
    /// \brief Holds amount of blocks.
    ///
    uint8_t blockCounter;

    ///
    /// \brief Holds total memory usage in EEPROM for current database layout.
    ///
    uint32_t memoryUsage;
};

/// @}