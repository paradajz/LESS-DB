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

#include "common.h"

namespace lib::lessdb
{
    class LessDb
    {
        public:
        LessDb(Hwa& hwa)
            : _hwa(hwa)
        {}

        bool            init();
        bool            setLayout(std::vector<Block>& layout, uint32_t startAddress = 0);
        static uint16_t layoutUid(std::vector<Block>& layout, uint16_t magicValue = 0);
        bool            clear();
        bool            read(size_t blockIndex, size_t sectionIndex, size_t parameterIndex, uint32_t& value);
        uint32_t        read(size_t blockIndex, size_t sectionIndex, size_t parameterIndex);
        bool            update(size_t blockIndex, size_t sectionIndex, size_t parameterIndex, uint32_t newValue);
        uint32_t        currentDatabaseSize() const;
        uint32_t        currentDatabaseParameters() const;
        uint32_t        dbSize() const;
        uint32_t        lastParameterAddress() const;
        uint32_t        nextParameterAddress() const;
        bool            initData(factoryResetType_t type = factoryResetType_t::FULL);

        private:
        /// Array holding all bit masks for easier access.
        static constexpr uint8_t BIT_MASK[8] = {
            0b00000001,
            0b00000010,
            0b00000100,
            0b00001000,
            0b00010000,
            0b00100000,
            0b01000000,
            0b10000000,
        };

        /// Reference to object which provides actual access to the storage system.
        Hwa& _hwa;

        /// Holds total memory usage for current database layout.
        uint32_t _memoryUsage = 0;

        /// Holds total number of parameters stored in database.
        uint32_t _memoryParameters = 0;

        /// Address from which database layout starts.
        uint32_t _initialAddress = 0;

        /// Database layout.
        std::vector<Block>* _layout = {};

        /// Cached values for bit and half-byte parameters.
        /// Used if current requested address is the same as previous one.
        uint8_t  _lastReadValue   = 0;
        uint32_t _lastReadAddress = 0xFFFFFFFF;

        /// Holds the database address at which last parameter is stored.
        uint32_t _nextBlockAddress = 0;

        bool     write(uint32_t address, uint32_t value, sectionParameterType_t type);
        bool     checkParameters(size_t blockIndex, size_t sectionIndex, size_t parameterIndex);
        uint32_t sectionAddress(size_t blockIndex, size_t sectionIndex);
    };
}    // namespace lib::lessdb
