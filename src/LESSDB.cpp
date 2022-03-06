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

#include <stdlib.h>
#include "LESSDB.h"

#define LAYOUT_ACCESS (*_layout)

bool LESSDB::init()
{
    return _storageAccess.init();
}

/// Calculates all addresses for specified blocks and sections.
/// param [in] layout           Reference to database structure.
/// param [in] startAddress     Address from which to start indexing blocks.
///                             Set to 0 by default.
/// returns: True on success, false otherwise.
bool LESSDB::setLayout(std::vector<Block>& layout, uint32_t startAddress)
{
    if (startAddress >= _storageAccess.size())
    {
        return false;
    }

    _initialAddress   = startAddress;
    _memoryUsage      = 0;
    _memoryParameters = 0;

    if (!layout.size())
    {
        return false;
    }

    _layout = &layout;

    for (size_t i = 0; i < LAYOUT_ACCESS.size(); i++)
    {
        uint32_t blockUsage = 0;

        for (size_t j = 0; j < LAYOUT_ACCESS[i]._sections.size(); j++)
        {
            if (!j)
            {
                // first section address is always 0
                LAYOUT_ACCESS[i]._sections[0]._address = 0;
            }
            else
            {
                size_t lastSection = j - 1;

                switch (LAYOUT_ACCESS[i]._sections[lastSection]._parameterType)
                {
                case sectionParameterType_t::bit:
                {
                    LAYOUT_ACCESS[i]._sections[j]._address =
                        (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters % 8 != 0) +
                        (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters / 8) +
                        LAYOUT_ACCESS[i]._sections[lastSection]._address;
                }
                break;

                case sectionParameterType_t::byte:
                {
                    LAYOUT_ACCESS[i]._sections[j]._address =
                        LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters +
                        LAYOUT_ACCESS[i]._sections[lastSection]._address;
                }
                break;

                case sectionParameterType_t::halfByte:
                {
                    LAYOUT_ACCESS[i]._sections[j]._address =
                        (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters % 2 != 0) +
                        (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters / 2) +
                        LAYOUT_ACCESS[i]._sections[lastSection]._address;
                }
                break;

                case sectionParameterType_t::word:
                {
                    LAYOUT_ACCESS[i]._sections[j]._address =
                        2 * LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters +
                        LAYOUT_ACCESS[i]._sections[lastSection]._address;
                }
                break;

                default:
                {
                    // case sectionParameterType_t::dword:
                    LAYOUT_ACCESS[i]._sections[j]._address =
                        4 * LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters +
                        LAYOUT_ACCESS[i]._sections[lastSection]._address;
                }
                break;
                }
            }

            _memoryParameters += LAYOUT_ACCESS[i]._sections[j]._numberOfParameters;

            switch (LAYOUT_ACCESS[i]._sections[j]._parameterType)
            {
            case sectionParameterType_t::bit:
            {
                blockUsage += (((LAYOUT_ACCESS[i]._sections[j]._numberOfParameters % 8 != 0) +
                                LAYOUT_ACCESS[i]._sections[j]._numberOfParameters / 8));
            }
            break;

            case sectionParameterType_t::halfByte:
            {
                blockUsage += (((LAYOUT_ACCESS[i]._sections[j]._numberOfParameters % 2 != 0) +
                                LAYOUT_ACCESS[i]._sections[j]._numberOfParameters / 2));
            }
            break;

            case sectionParameterType_t::byte:
            case sectionParameterType_t::word:
            case sectionParameterType_t::dword:
            default:
            {
                const size_t multiplier = (LAYOUT_ACCESS[i]._sections[j]._parameterType == sectionParameterType_t::byte)
                                              ? 1
                                          : (LAYOUT_ACCESS[i]._sections[j]._parameterType == sectionParameterType_t::word)
                                              ? 2     // word
                                              : 4;    // dword

                blockUsage += (multiplier *
                               LAYOUT_ACCESS[i]._sections[j]._numberOfParameters);
            }
            break;
            }
        }

        _memoryUsage += blockUsage;

        if (_memoryUsage >= _storageAccess.size())
        {
            return false;
        }

        size_t lastSection = LAYOUT_ACCESS[i]._sections.size() - 1;

        if (!i)
        {
            LAYOUT_ACCESS[0]._address = _initialAddress;
        }

        _nextBlockAddress = LAYOUT_ACCESS[i]._address + LAYOUT_ACCESS[i]._sections[lastSection]._address;

        switch (LAYOUT_ACCESS[i]._sections[lastSection]._parameterType)
        {
        case sectionParameterType_t::bit:
        {
            _nextBlockAddress +=
                (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters % 8 != 0) +
                (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters / 8);
        }
        break;

        case sectionParameterType_t::byte:
        {
            _nextBlockAddress +=
                LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters;
        }
        break;

        case sectionParameterType_t::halfByte:
        {
            _nextBlockAddress +=
                (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters % 2 != 0) +
                (LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters / 2);
        }
        break;

        case sectionParameterType_t::word:
        {
            _nextBlockAddress +=
                2 * LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters;
        }
        break;

        default:
        {
            // case sectionParameterType_t::dword:
            _nextBlockAddress +=
                4 * LAYOUT_ACCESS[i]._sections[lastSection]._numberOfParameters;
        }
        break;
        }

        if (i < (LAYOUT_ACCESS.size() - 1))
        {
            LAYOUT_ACCESS[i + 1]._address = _nextBlockAddress;
        }
    }

    return true;
}

/// Calculates unique ID for specified layout.
/// UID is calculated by appending number of parameters and their types for all
/// sections and all blocks.
/// param [in] layout       Reference to database structure.
/// param [in] magicValue   Additional optional value which will be appended
///                         to calculated UID. If ommited, it is set to 0
///                         by default.
uint16_t LESSDB::layoutUID(std::vector<Block>& layout, uint16_t magicValue)
{
    if (!layout.size())
    {
        return 0;
    }

    uint16_t signature = 0;

    // get unique database signature based on its blocks/sections
    for (size_t i = 0; i < layout.size(); i++)
    {
        for (size_t j = 0; j < layout[i]._sections.size(); j++)
        {
            signature += static_cast<uint16_t>(layout[i]._sections[j]._numberOfParameters);
            signature += static_cast<uint16_t>(layout[i]._sections[j]._parameterType);
        }
    }

    signature += magicValue;

    return signature;
}

/// Reads a value from database.
/// param [in] blockID         Block index.
/// param [in] sectionID       Section index.
/// param [in] parameterIndex  Parameter index.
/// param [in, out] value      Reference to variable in which read value will be stored.
/// returns: True on success.
bool LESSDB::read(size_t blockID, size_t sectionID, size_t parameterIndex, int32_t& value)
{
    // sanity checks
    if (!checkParameters(blockID, sectionID, parameterIndex))
    {
        return false;
    }

    bool     returnValue  = true;
    uint32_t startAddress = LAYOUT_ACCESS[blockID]._address + LAYOUT_ACCESS[blockID]._sections[sectionID]._address;
    uint8_t  arrayIndex;

    switch (LAYOUT_ACCESS[blockID]._sections[sectionID]._parameterType)
    {
    case sectionParameterType_t::bit:
    {
        arrayIndex = parameterIndex >> 3;
        startAddress += arrayIndex;

        if (startAddress == _lastReadAddress)
        {
            value = (bool)(_lastReadValue & BIT_MASK[parameterIndex - (arrayIndex << 3)]);
        }
        else if (_storageAccess.read(startAddress, value, sectionParameterType_t::bit))
        {
            _lastReadValue = value;
            value          = (bool)(value & BIT_MASK[parameterIndex - (arrayIndex << 3)]);
        }
        else
        {
            returnValue = false;
        }
    }
    break;

    case sectionParameterType_t::byte:
    {
        startAddress += parameterIndex;

        if (_storageAccess.read(startAddress, value, sectionParameterType_t::byte))
        {
            // sanitize
            value &= (int32_t)0xFF;
        }
        else
        {
            returnValue = false;
        }
    }
    break;

    case sectionParameterType_t::halfByte:
    {
        startAddress += parameterIndex / 2;

        if (startAddress == _lastReadAddress)
        {
            value = _lastReadValue;

            if (parameterIndex % 2)
            {
                value >>= 4;
            }
        }
        else if (_storageAccess.read(startAddress, value, sectionParameterType_t::halfByte))
        {
            _lastReadValue = value;

            if (parameterIndex % 2)
            {
                value >>= 4;
            }
        }
        else
        {
            returnValue = false;
        }

        if (returnValue)
        {
            // sanitize
            value &= (int32_t)0x0F;
        }
    }
    break;

    case sectionParameterType_t::word:
    {
        startAddress += parameterIndex * 2;

        if (_storageAccess.read(startAddress, value, sectionParameterType_t::word))
        {
            // sanitize
            value &= (int32_t)0xFFFF;
        }
        else
        {
            returnValue = false;
        }
    }
    break;

    default:
    {
        // case sectionParameterType_t::dword:
        startAddress += parameterIndex * 4;
        return _storageAccess.read(startAddress, value, sectionParameterType_t::dword);
    }
    break;
    }

    if (returnValue)
    {
        _lastReadAddress = startAddress;
    }

    return returnValue;
}

/// Reads a value from database with reduced error checking.
/// param [in] blockID         Block index.
/// param [in] sectionID       Section index.
/// param [in] parameterIndex  Parameter index.
/// returns: Value from database. In case of read failure, 0 will be returned.
int32_t LESSDB::read(size_t blockID, size_t sectionID, size_t parameterIndex)
{
    int32_t value = 0;
    read(blockID, sectionID, parameterIndex, value);
    return value;
}

/// Updates value for specified block and section in database.
/// param [in] blockID         Block index.
/// param [in] sectionID       Section index.
/// param [in] parameterIndex  Parameter index.
/// param [in] newValue        New value for parameter.
/// returns: True on success, false otherwise.
bool LESSDB::update(size_t blockID, size_t sectionID, size_t parameterIndex, int32_t newValue)
{
    if (!LAYOUT_ACCESS.size())
    {
        return false;
    }

    // sanity check
    if (!checkParameters(blockID, sectionID, parameterIndex))
    {
        return false;
    }

    uint32_t               startAddress  = sectionAddress(blockID, sectionID);
    sectionParameterType_t parameterType = LAYOUT_ACCESS[blockID]._sections[sectionID]._parameterType;

    uint8_t arrayIndex;
    int32_t arrayValue;
    uint8_t bitIndex;

    switch (parameterType)
    {
    case sectionParameterType_t::bit:
    {
        // reset cached address to initiate new read
        _lastReadAddress = 0xFFFFFFFF;
        // sanitize input
        newValue &= (int32_t)0x01;
        arrayIndex = parameterIndex / 8;
        bitIndex   = parameterIndex - 8 * arrayIndex;
        startAddress += arrayIndex;

        // read existing value first
        if (_storageAccess.read(startAddress, arrayValue, sectionParameterType_t::bit))
        {
            // update value with new bit
            if (newValue)
            {
                arrayValue |= BIT_MASK[bitIndex];
            }
            else
            {
                arrayValue &= ~BIT_MASK[bitIndex];
            }

            return write(startAddress, arrayValue, sectionParameterType_t::bit);
        }
    }
    break;

    case sectionParameterType_t::byte:
    {
        // sanitize input
        newValue &= (int32_t)0xFF;
        startAddress += parameterIndex;
        return write(startAddress, newValue, sectionParameterType_t::byte);
    }
    break;

    case sectionParameterType_t::halfByte:
    {
        // reset cached address to initiate new read
        _lastReadAddress = 0xFFFFFFFF;
        // sanitize input
        newValue &= (int32_t)0x0F;
        startAddress += (parameterIndex / 2);

        // read old value first
        if (_storageAccess.read(startAddress, arrayValue, sectionParameterType_t::halfByte))
        {
            if (parameterIndex % 2)
            {
                // clear content in bits 4-7 and update the value
                arrayValue &= 0x0F;
                arrayValue |= (newValue << 4);
            }
            else
            {
                // clear content in bits 0-3 and update the value
                arrayValue &= 0xF0;
                arrayValue |= newValue;
            }

            return write(startAddress, arrayValue, sectionParameterType_t::halfByte);
        }
    }
    break;

    case sectionParameterType_t::word:
    {
        // sanitize input
        newValue &= (int32_t)0xFFFF;
        startAddress += (parameterIndex * 2);
        return write(startAddress, newValue, sectionParameterType_t::word);
    }
    break;

    case sectionParameterType_t::dword:
    {
        startAddress += (parameterIndex * 4);
        return write(startAddress, newValue, sectionParameterType_t::dword);
    }
    break;
    }

    return false;
}

/// Convenience function to write value at specified address.
/// param [in] address Address to which to write the variable.
/// param [in] value   Value to write.
/// param [in] type    Type of variable.
/// returns: True if writing succedes and read value matches the specified value, false otherwise.
bool LESSDB::write(uint32_t address, int32_t value, sectionParameterType_t type)
{
    if (_storageAccess.write(address, value, type))
    {
        int32_t readValue;

        if (_storageAccess.read(address, readValue, type))
        {
            return (value == readValue);
        }
    }

    return false;
}

/// Clears entire memory.
bool LESSDB::clear()
{
    return _storageAccess.clear();
}

/// Writes default values to memory from defaultValue parameter.
/// param [in] type     Type of initialization (partial or full).
///                     Full will simply overwrite currently existing data.
///                     Partial will leave data as is, but only if
///                     preserveOnPartialReset parameter is set to true.
bool LESSDB::initData(factoryResetType_t type)
{
    for (size_t i = 0; i < LAYOUT_ACCESS.size(); i++)
    {
        for (size_t j = 0; j < LAYOUT_ACCESS[i]._sections.size(); j++)
        {
            if ((LAYOUT_ACCESS[i]._sections[j]._preserveOnPartialReset == preserveSetting_t::enable) && (type == factoryResetType_t::partial))
            {
                continue;
            }

            uint32_t startAddress = sectionAddress(i, j);

            auto parameterType      = LAYOUT_ACCESS[i]._sections[j]._parameterType;
            auto defaultValue       = LAYOUT_ACCESS[i]._sections[j]._defaultValue;
            auto numberOfParameters = LAYOUT_ACCESS[i]._sections[j]._numberOfParameters;

            switch (parameterType)
            {
            case sectionParameterType_t::byte:
            case sectionParameterType_t::word:
            case sectionParameterType_t::dword:
            {
                for (size_t k = 0; k < numberOfParameters; k++)
                {
                    if (LAYOUT_ACCESS[i]._sections[j]._autoIncrement == autoIncrementSetting_t::enable)
                    {
                        if (!write(startAddress, defaultValue + k, parameterType))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (!write(startAddress, defaultValue, parameterType))
                        {
                            return false;
                        }
                    }

                    if (parameterType == sectionParameterType_t::byte)
                    {
                        startAddress++;
                    }
                    else if (parameterType == sectionParameterType_t::word)
                    {
                        startAddress += 2;
                    }
                    else if (parameterType == sectionParameterType_t::dword)
                    {
                        startAddress += 4;
                    }
                }
            }
            break;

            case sectionParameterType_t::bit:
            {
                // no auto-increment here
                size_t loops = (numberOfParameters / 8) + ((numberOfParameters % 8) != 0);

                // optimize the writing - merge values into single byte
                for (size_t k = 0; k < loops; k++)
                {
                    uint8_t value = 0;

                    for (uint8_t parameter = 0; parameter < 8; parameter++)
                    {
                        if (((k * 8) + parameter) >= numberOfParameters)
                        {
                            break;
                        }

                        value <<= 1;
                        value |= defaultValue;
                    }

                    if (!write(startAddress, value, sectionParameterType_t::byte))
                    {
                        return false;
                    }

                    startAddress++;
                }
            }
            break;

            case sectionParameterType_t::halfByte:
            {
                // no auto-increment here
                size_t loops = (numberOfParameters / 2) + ((numberOfParameters % 2) != 0);

                // optimize the writing - merge values into single byte
                for (size_t k = 0; k < loops; k++)
                {
                    uint8_t value = 0;

                    for (uint8_t parameter = 0; parameter < 2; parameter++)
                    {
                        if (((k * 2) + parameter) >= numberOfParameters)
                        {
                            break;
                        }

                        value <<= 4;
                        value |= defaultValue;
                    }

                    if (!write(startAddress, value, sectionParameterType_t::byte))
                    {
                        return false;
                    }

                    startAddress++;
                }
            }
            break;
            }
        }
    }

    return true;
}

/// Checks for total memory usage of database.
/// returns: Database size in bytes.
uint32_t LESSDB::currentDBsize() const
{
    return _memoryUsage;
}

/// Checks for total amount of parameters stored in database.
/// returns: Number of parameters.
uint32_t LESSDB::currentDBparameters() const
{
    return _memoryParameters;
}

/// Retrieves maximum database size.
/// returns: Maximum database size in bytes.
uint32_t LESSDB::dbSize() const
{
    return _storageAccess.size();
}

/// Returns the database address at which last parameter is stored.
uint32_t LESSDB::lastParameterAddress() const
{
    return _nextBlockAddress - 1;
}

/// Returns first unused database address.
uint32_t LESSDB::nextParameterAddress() const
{
    return _nextBlockAddress;
}

/// Validates input parameters before attempting to read or write data.
/// param [in] blockID         Block index.
/// param [in] sectionID       Section index.
/// param [in] parameterID     Parameter index.
/// returns: True if parameters are valid, false otherwise.
bool LESSDB::checkParameters(size_t blockID, size_t sectionID, size_t parameterIndex)
{
    // sanity check
    if (blockID >= LAYOUT_ACCESS.size())
    {
        return false;
    }

    if (sectionID >= LAYOUT_ACCESS[blockID]._sections.size())
    {
        return false;
    }

    if (parameterIndex >= LAYOUT_ACCESS[blockID]._sections[sectionID]._numberOfParameters)
    {
        return false;
    }

    return true;
}

/// Returns section address for specified section within block.
/// param [in] blockID     Block index.
/// param [in] sectionID   Section index.
/// returns: Section address.
uint32_t LESSDB::sectionAddress(size_t blockID, size_t sectionID)
{
    return LAYOUT_ACCESS[blockID]._address + LAYOUT_ACCESS[blockID]._sections[sectionID]._address;
}