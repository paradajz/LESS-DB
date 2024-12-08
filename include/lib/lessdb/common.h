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

namespace lib::lessdb
{
    class LessDb;

    enum class sectionParameterType_t : uint8_t
    {
        BIT,
        BYTE,
        HALF_BYTE,
        WORD,
        DWORD
    };

    class Hwa
    {
        public:
        virtual bool     init()                                                               = 0;
        virtual uint32_t size()                                                               = 0;
        virtual bool     clear()                                                              = 0;
        virtual bool     read(uint32_t address, uint32_t& value, sectionParameterType_t type) = 0;
        virtual bool     write(uint32_t address, uint32_t value, sectionParameterType_t type) = 0;
    };

    enum class factoryResetType_t : uint8_t
    {
        PARTIAL,
        FULL
    };

    enum class preserveSetting_t : uint8_t
    {
        ENABLE,
        DISABLE
    };

    enum class autoIncrementSetting_t : uint8_t
    {
        ENABLE,
        DISABLE
    };

    class Section
    {
        public:
        Section(size_t                 numberOfParameters,
                sectionParameterType_t parameterType,
                preserveSetting_t      preserveOnPartialReset,
                autoIncrementSetting_t autoIncrement,
                uint32_t               defaultValue)
            : NUMBER_OF_PARAMETERS(numberOfParameters)
            , PARAMETER_TYPE(parameterType)
            , PRESERVE_ON_PARTIAL_RESET(preserveOnPartialReset)
            , AUTO_INCREMENT(autoIncrement)
            , DEFAULT_VALUE(defaultValue)
            , DEFAULT_VALUES(std::vector<uint32_t>{})
        {}

        Section(size_t                 numberOfParameters,
                sectionParameterType_t parameterType,
                preserveSetting_t      preserveOnPartialReset,
                autoIncrementSetting_t autoIncrement,
                std::vector<uint32_t>  defaultValues)
            : NUMBER_OF_PARAMETERS(numberOfParameters)
            , PARAMETER_TYPE(parameterType)
            , PRESERVE_ON_PARTIAL_RESET(preserveOnPartialReset)
            , AUTO_INCREMENT(autoIncrement)
            , DEFAULT_VALUE(0)
            , DEFAULT_VALUES(std::move(defaultValues))
        {}

        private:
        friend class LessDb;

        const size_t                 NUMBER_OF_PARAMETERS;
        const sectionParameterType_t PARAMETER_TYPE;
        const preserveSetting_t      PRESERVE_ON_PARTIAL_RESET;
        const autoIncrementSetting_t AUTO_INCREMENT;
        const uint32_t               DEFAULT_VALUE;
        const std::vector<uint32_t>  DEFAULT_VALUES;
        uint32_t                     _address = 0;
    };

    class Block
    {
        public:
        Block(std::vector<Section>& sections)
            : _sections(sections)
        {}

        private:
        friend class LessDb;

        std::vector<Section>& _sections;
        uint32_t              _address = 0;
    };
}    // namespace lib::lessdb
