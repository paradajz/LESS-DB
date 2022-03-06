/*
    Copyright 2017-2020 Igor Petrovic

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
#include <vector>

class LESSDB
{
    public:
    enum class sectionParameterType_t : uint8_t
    {
        bit,
        byte,
        halfByte,
        word,
        dword
    };

    class StorageAccess
    {
        public:
        virtual bool     init()                                                                      = 0;
        virtual uint32_t size()                                                                      = 0;
        virtual bool     clear()                                                                     = 0;
        virtual bool     read(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) = 0;
        virtual bool     write(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) = 0;
    };

    enum class factoryResetType_t : uint8_t
    {
        partial,
        full
    };

    enum class preserveSetting_t : uint8_t
    {
        enable,
        disable
    };

    enum class autoIncrementSetting_t : uint8_t
    {
        enable,
        disable
    };

    class Section
    {
        public:
        Section(size_t                 numberOfParameters,
                sectionParameterType_t parameterType,
                preserveSetting_t      preserveOnPartialReset,
                autoIncrementSetting_t autoIncrement,
                int32_t                defaultValue)
            : _numberOfParameters(numberOfParameters)
            , _parameterType(parameterType)
            , _preserveOnPartialReset(preserveOnPartialReset)
            , _autoIncrement(autoIncrement)
            , _defaultValue(defaultValue)
        {}

        private:
        friend class LESSDB;

        const size_t                 _numberOfParameters;
        const sectionParameterType_t _parameterType;
        const preserveSetting_t      _preserveOnPartialReset;
        const autoIncrementSetting_t _autoIncrement;
        const int32_t                _defaultValue;
        uint32_t                     _address = 0;
    };

    class Block
    {
        public:
        Block(std::vector<Section>& sections)
            : _sections(sections)
        {}

        private:
        friend class LESSDB;

        std::vector<Section>& _sections;
        uint32_t              _address = 0;
    };

    LESSDB(StorageAccess& storageAccess)
        : storageAccess(storageAccess)
    {}

    bool            init();
    bool            setLayout(std::vector<Block>& layout, uint32_t startAddress);
    static uint16_t layoutUID(std::vector<Block>& layout, uint16_t magicValue = 0);
    bool            clear();
    bool            read(size_t blockID, size_t sectionID, size_t parameterIndex, int32_t& value);
    int32_t         read(size_t blockID, size_t sectionID, size_t parameterIndex);
    bool            update(size_t blockID, size_t sectionID, size_t parameterIndex, int32_t newValue);
    uint32_t        currentDBsize() const;
    uint32_t        currentDBparameters() const;
    uint32_t        dbSize() const;
    uint32_t        lastParameterAddress() const;
    uint32_t        nextParameterAddress() const;
    bool            initData(factoryResetType_t type = factoryResetType_t::full);

    private:
    bool     write(uint32_t address, int32_t value, sectionParameterType_t type);
    bool     checkParameters(size_t blockID, size_t sectionID, size_t parameterIndex);
    uint32_t sectionAddress(size_t blockID, size_t sectionID);

    /// Holds total memory usage for current database layout.
    uint32_t memoryUsage = 0;

    /// Holds total number of parameters stored in database.
    uint32_t memoryParameters = 0;

    /// Address from which database layout starts.
    uint32_t initialAddress = 0;

    /// Array holding all bit masks for easier access.
    const uint8_t bitMask[8] = {
        0b00000001,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b10000000,
    };

    /// Database layout.
    std::vector<Block>* _layout = {};

    /// Reference to object which provides actual access to the storage system.
    StorageAccess& storageAccess;

    /// Cached values for bit and half-byte parameters.
    /// Used if current requested address is the same as previous one.
    uint8_t  lastReadValue   = 0;
    uint32_t lastReadAddress = 0xFFFFFFFF;

    /// Holds the database address at which last parameter is stored.
    uint32_t nextBlockAddress = 0;
};