/*
    Copyright 2017-2019 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#pragma once

#include <inttypes.h>
#include <stddef.h>

class LESSDB
{
    public:
    ///
    /// \brief List of possible parameter types in section.
    ///
    enum class sectionParameterType_t : uint8_t
    {
        bit,
        byte,
        halfByte,
        word,
        dword
    };

    ///
    /// \brief List of possible ways to perform factory reset.
    ///
    enum class factoryResetType_t : uint8_t
    {
        partial,
        full
    };

    ///
    /// \brief A structure holding information for a single section.
    ///
    typedef struct
    {
        int16_t                numberOfParameters;
        sectionParameterType_t parameterType;
        bool                   preserveOnPartialReset;
        uint16_t               defaultValue;
        bool                   autoIncrement;
        int16_t                address;
    } section_t;

    ///
    /// \brief A structure holding information for a single block.
    ///
    typedef struct
    {
        int16_t    address;
        int8_t     numberOfSections;
        section_t* section;
    } block_t;

    ///
    /// \brief LESSDB constructor.
    /// @param [in] readCallback    Reference to function performing the actual reading from memory.
    /// @param [in] writeCallback   Reference to function performing the actual writing to memory.
    /// @param [in] maxSize         Specifies maximum database size in bytes.
    ///
    LESSDB(bool (&readCallback)(uint32_t address, sectionParameterType_t type, int32_t& value),
           bool (&writeCallback)(uint32_t address, int32_t value, sectionParameterType_t type),
           size_t maxSize)
        : readCallback(readCallback)
        , writeCallback(writeCallback)
        , maxSize(maxSize)
    {}

    bool     setLayout(block_t* pointer, uint8_t numberOfBlocks);
    bool     clear();
    bool     read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t& value);
    int32_t  read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    bool     update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue);
    uint32_t currentDBusage() const;
    size_t   dbSize() const;
    bool     initData(factoryResetType_t type = factoryResetType_t::full);
    bool     setStartAddress(uint16_t startAddress);

    private:
    bool     checkParameters(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex);
    uint16_t sectionAddress(uint8_t blockID, uint8_t sectionID);

    ///
    /// \brief Holds amount of blocks.
    ///
    uint8_t blockCounter = 0;

    ///
    /// \brief Holds total memory usage for current database layout.
    ///
    uint32_t memoryUsage = 0;

    ///
    /// \brief Address from which database layout starts.
    ///
    uint16_t initialAddress = 0;

    ///
    /// \brief Array holding all bit masks for easier access.
    ///
    const uint8_t bitMask[8] = { 0b00000001, 0b00000010, 0b00000100, 0b00001000,
                                 0b00010000, 0b00100000, 0b01000000, 0b10000000 };

    ///
    /// \brief Pointer to array of LESSDB blocks.
    ///
    block_t* block = nullptr;

    ///
    /// \brief Function reference used to read the memory contents.
    ///
    bool (&readCallback)(uint32_t address, sectionParameterType_t type, int32_t& value);

    ///
    /// \brief Function reference used to update the memory contents.
    ///
    bool (&writeCallback)(uint32_t address, int32_t value, sectionParameterType_t type);

    ///
    /// \brief Holds the maximum size of database system in bytes.
    ///
    const size_t maxSize;

    ///
    /// \brief Cached values for bit and half-byte parameters.
    /// Used if current requested address is the same as previous one.
    /// @{

    uint8_t  lastValue = 0;
    uint16_t lastAddress = maxSize;

    /// @}
};