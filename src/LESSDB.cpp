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

bool LESSDB::init()
{
    return storageAccess.init();
}

///
/// \brief Calculates all addresses for specified blocks and sections.
/// @param [in] pointer         Pointer to database structure.
/// @param [in] numberOfBlocks  Total number of blocks in database structure.
/// \returns True on success, false otherwise.
///
bool LESSDB::setLayout(block_t* pointer, uint8_t numberOfBlocks)
{
    memoryUsage      = 0;
    memoryParameters = 0;

    if ((pointer != nullptr) && numberOfBlocks)
    {
        block        = pointer;
        blockCounter = numberOfBlocks;

        for (int i = 0; i < blockCounter; i++)
        {
            uint32_t blockUsage = 0;

            for (int j = 0; j < block[i].numberOfSections; j++)
            {
                if (!j)
                {
                    // first section address is always 0
                    block[i].section[0].address = 0;
                }
                else
                {
                    uint8_t lastSection = j - 1;

                    switch (block[i].section[lastSection].parameterType)
                    {
                    case sectionParameterType_t::bit:
                        block[i].section[j].address =
                            (block[i].section[lastSection].numberOfParameters % 8 != 0) +
                            (block[i].section[lastSection].numberOfParameters / 8) +
                            block[i].section[lastSection].address;
                        break;

                    case sectionParameterType_t::byte:
                        block[i].section[j].address =
                            block[i].section[lastSection].numberOfParameters +
                            block[i].section[lastSection].address;
                        break;

                    case sectionParameterType_t::halfByte:
                        block[i].section[j].address =
                            (block[i].section[lastSection].numberOfParameters % 2 != 0) +
                            (block[i].section[lastSection].numberOfParameters / 2) +
                            block[i].section[lastSection].address;
                        break;

                    case sectionParameterType_t::word:
                        block[i].section[j].address =
                            2 * block[i].section[lastSection].numberOfParameters +
                            block[i].section[lastSection].address;
                        break;

                    default:
                        // case sectionParameterType_t::dword:
                        block[i].section[j].address =
                            4 * block[i].section[lastSection].numberOfParameters +
                            block[i].section[lastSection].address;
                        break;
                    }
                }

                memoryParameters += block[i].section[j].numberOfParameters;

                switch (block[i].section[j].parameterType)
                {
                case sectionParameterType_t::bit:
                    blockUsage += (storageAccess.paramUsage(sectionParameterType_t::bit) *
                                   ((block[i].section[j].numberOfParameters % 8 != 0) +
                                    block[i].section[j].numberOfParameters / 8));
                    break;

                case sectionParameterType_t::halfByte:
                    blockUsage += (storageAccess.paramUsage(sectionParameterType_t::halfByte) *
                                   ((block[i].section[j].numberOfParameters % 2 != 0) +
                                    block[i].section[j].numberOfParameters / 2));
                    break;

                case sectionParameterType_t::byte:
                case sectionParameterType_t::word:
                case sectionParameterType_t::dword:
                default:
                    blockUsage += (storageAccess.paramUsage(block[i].section[j].parameterType) *
                                   block[i].section[j].numberOfParameters);
                    break;
                }
            }

            memoryUsage += blockUsage;

            if (memoryUsage >= storageAccess.size())
                return false;

            uint16_t lastSection = block[i].numberOfSections - 1;

            nextBlockAddress = block[i].address + block[i].section[lastSection].address;

            switch (block[i].section[lastSection].parameterType)
            {
            case sectionParameterType_t::bit:
                nextBlockAddress +=
                    (block[i].section[lastSection].numberOfParameters % 8 != 0) +
                    (block[i].section[lastSection].numberOfParameters / 8);
                break;

            case sectionParameterType_t::byte:
                nextBlockAddress +=
                    block[i].section[lastSection].numberOfParameters;
                break;

            case sectionParameterType_t::halfByte:
                nextBlockAddress +=
                    (block[i].section[lastSection].numberOfParameters % 2 != 0) +
                    (block[i].section[lastSection].numberOfParameters / 2);
                break;

            case sectionParameterType_t::word:
                nextBlockAddress +=
                    2 * block[i].section[lastSection].numberOfParameters;
                break;

            default:
                // case sectionParameterType_t::dword:
                nextBlockAddress +=
                    4 * block[i].section[lastSection].numberOfParameters;
                break;
            }

            if (!i)
                block[0].address = 0;

            if (i < (blockCounter - 1))
                block[i + 1].address = nextBlockAddress;
        }

        return true;
    }

    return false;
}

///
/// \brief Reads a value from database.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// @param [in, out] value      Reference to variable in which read value will be stored.
/// \returns True on success.
///
bool LESSDB::read(uint8_t blockID, uint8_t sectionID, size_t parameterIndex, int32_t& value)
{
    // sanity check
    if (!checkParameters(blockID, sectionID, parameterIndex))
        return false;

    bool     returnValue  = true;
    uint32_t startAddress = sectionAddress(blockID, sectionID);
    uint8_t  arrayIndex;

    switch (block[blockID].section[sectionID].parameterType)
    {
    case sectionParameterType_t::bit:
        arrayIndex = parameterIndex >> 3;
        startAddress += arrayIndex;

        if (startAddress == lastReadAddress)
        {
            value = (bool)(lastReadValue & bitMask[parameterIndex - (arrayIndex << 3)]);
        }
        else if (storageAccess.read(startAddress, value, sectionParameterType_t::bit))
        {
            lastReadValue = value;
            value         = (bool)(value & bitMask[parameterIndex - (arrayIndex << 3)]);
        }
        else
        {
            returnValue = false;
        }
        break;

    case sectionParameterType_t::byte:
        startAddress += parameterIndex;

        if (storageAccess.read(startAddress, value, sectionParameterType_t::byte))
        {
            // sanitize
            value &= (int32_t)0xFF;
        }
        else
        {
            returnValue = false;
        }
        break;

    case sectionParameterType_t::halfByte:
        startAddress += parameterIndex / 2;

        if (startAddress == lastReadAddress)
        {
            value = lastReadValue;

            if (parameterIndex % 2)
                value >>= 4;
        }
        else if (storageAccess.read(startAddress, value, sectionParameterType_t::halfByte))
        {
            lastReadValue = value;

            if (parameterIndex % 2)
                value >>= 4;
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
        break;

    case sectionParameterType_t::word:
        startAddress += parameterIndex * 2;

        if (storageAccess.read(startAddress, value, sectionParameterType_t::word))
        {
            // sanitize
            value &= (int32_t)0xFFFF;
        }
        else
        {
            returnValue = false;
        }
        break;

    default:
        // case sectionParameterType_t::dword:
        startAddress += parameterIndex * 4;
        return storageAccess.read(startAddress, value, sectionParameterType_t::dword);
        break;
    }

    if (returnValue)
        lastReadAddress = startAddress;

    return returnValue;
}

///
/// \brief Reads a value from database.
/// @param [in] address         Database address from which to read.
/// @param [in, out] value      Reference to variable in which read value will be stored.
/// @param [in] type            Type of value to read.
/// \returns True on success.
///
bool LESSDB::read(uint32_t address, int32_t& value, sectionParameterType_t type)
{
    return storageAccess.read(address, value, type);
}

///
/// \brief Reads a value from database without error checking.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// \returns Value from database.
///
int32_t LESSDB::read(uint8_t blockID, uint8_t sectionID, size_t parameterIndex)
{
    int32_t value = 0;

    read(blockID, sectionID, parameterIndex, value);

    return value;
}

///
/// \brief Reads a byte value from database without error checking.
/// @param [in] address         Database address from which to read.
/// @param [in] type            Type of value to read.
/// \returns Value from database.
///
int32_t LESSDB::read(uint32_t address, sectionParameterType_t type)
{
    int32_t value = -1;

    read(address, value, type);

    return value;
}

///
/// \brief Updates value for specified block and section in database.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// @param [in] newValue        New value for parameter.
/// \returns True on success, false otherwise.
///
bool LESSDB::update(uint8_t blockID, uint8_t sectionID, size_t parameterIndex, int32_t newValue)
{
    if (block == nullptr)
        return false;

    // sanity check
    if (!checkParameters(blockID, sectionID, parameterIndex))
        return false;

    uint32_t               startAddress  = sectionAddress(blockID, sectionID);
    sectionParameterType_t parameterType = block[blockID].section[sectionID].parameterType;

    uint8_t arrayIndex;
    int32_t arrayValue;
    uint8_t bitIndex;

    switch (parameterType)
    {
    case sectionParameterType_t::bit:
        // reset cached address to initiate new read
        lastReadAddress = 0xFFFFFFFF;
        // sanitize input
        newValue &= (int32_t)0x01;
        arrayIndex = parameterIndex / 8;
        bitIndex   = parameterIndex - 8 * arrayIndex;
        startAddress += arrayIndex;

        // read existing value first
        if (storageAccess.read(startAddress, arrayValue, sectionParameterType_t::bit))
        {
            // update value with new bit
            if (newValue)
                arrayValue |= bitMask[bitIndex];
            else
                arrayValue &= ~bitMask[bitIndex];

            return write(startAddress, arrayValue, sectionParameterType_t::bit);
        }
        break;

    case sectionParameterType_t::byte:
        // sanitize input
        newValue &= (int32_t)0xFF;
        startAddress += parameterIndex;
        return write(startAddress, newValue, sectionParameterType_t::byte);
        break;

    case sectionParameterType_t::halfByte:
        // reset cached address to initiate new read
        lastReadAddress = 0xFFFFFFFF;
        // sanitize input
        newValue &= (int32_t)0x0F;
        startAddress += (parameterIndex / 2);

        // read old value first
        if (storageAccess.read(startAddress, arrayValue, sectionParameterType_t::halfByte))
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
        break;

    case sectionParameterType_t::word:
        // sanitize input
        newValue &= (int32_t)0xFFFF;
        startAddress += (parameterIndex * 2);
        return write(startAddress, newValue, sectionParameterType_t::word);
        break;

    case sectionParameterType_t::dword:
        startAddress += (parameterIndex * 4);
        return write(startAddress, newValue, sectionParameterType_t::dword);
        break;
    }

    return false;
}

///
/// \brief Updates value for specified database address.
/// @param [in] address     Address to which to write the value.
/// @param [in] newValue    New value for specified address.
/// @param [in] type        Type of value to write at specified address.
/// \returns True on success, false otherwise.
///
bool LESSDB::update(uint32_t address, int32_t newValue, sectionParameterType_t type)
{
    return write(address, newValue, type);
}

///
/// \brief Convenience function to write value at specified address.
/// @param [in] address Address to which to write the variable.
/// @param [in] value   Value to write.
/// @param [in] type    Type of variable.
/// \returns True if writing succedes and read value matches the specified value, false otherwise.
///
bool LESSDB::write(uint32_t address, int32_t value, sectionParameterType_t type)
{
    if (storageAccess.write(address, value, type))
    {
        int32_t readValue;

        if (storageAccess.read(address, readValue, type))
            return (value == readValue);
    }

    return false;
}

///
/// \brief Clears entire memory.
///
bool LESSDB::clear()
{
    return storageAccess.clear();
}

///
/// \brief Writes default values to memory from defaultValue parameter.
/// @param [in] type    Type of initialization (partial or full).
///                     Full will simply overwrite currently existing data.
///                     Partial will leave data as is, but only if
///                     preserveOnPartialReset parameter is set to true.
///
bool LESSDB::initData(factoryResetType_t type)
{
    for (uint8_t i = 0; i < blockCounter; i++)
    {
        for (uint8_t j = 0; j < block[i].numberOfSections; j++)
        {
            if (block[i].section[j].preserveOnPartialReset && (type == factoryResetType_t::partial))
                continue;

            uint32_t startAddress = sectionAddress(i, j);

            sectionParameterType_t parameterType      = block[i].section[j].parameterType;
            uint16_t               defaultValue       = block[i].section[j].defaultValue;
            size_t                 numberOfParameters = block[i].section[j].numberOfParameters;

            switch (parameterType)
            {
            case sectionParameterType_t::byte:
            case sectionParameterType_t::word:
            case sectionParameterType_t::dword:
            {
                for (size_t k = 0; k < numberOfParameters; k++)
                {
                    if (block[i].section[j].autoIncrement)
                    {
                        if (!write(startAddress, defaultValue + k, parameterType))
                            return false;
                    }
                    else
                    {
                        if (!write(startAddress, defaultValue, parameterType))
                            return false;
                    }

                    if (parameterType == sectionParameterType_t::byte)
                        startAddress++;
                    else if (parameterType == sectionParameterType_t::word)
                        startAddress += 2;
                    else if (parameterType == sectionParameterType_t::dword)
                        startAddress += 4;
                }
            }
            break;

            case sectionParameterType_t::bit:
            {
                // no auto-increment here
                size_t loops = (numberOfParameters / 8) + ((numberOfParameters % 8) != 0);

                //optimize the writing - merge values into single byte
                for (size_t k = 0; k < loops; k++)
                {
                    uint8_t value = 0;

                    for (uint8_t parameter = 0; parameter < 8; parameter++)
                    {
                        if (((k * 8) + parameter) >= numberOfParameters)
                            break;

                        value <<= 1;
                        value |= defaultValue;
                    }

                    if (!write(startAddress, value, sectionParameterType_t::byte))
                        return false;

                    startAddress++;
                }
            }
            break;

            case sectionParameterType_t::halfByte:
            {
                // no auto-increment here
                size_t loops = (numberOfParameters / 2) + ((numberOfParameters % 2) != 0);

                //optimize the writing - merge values into single byte
                for (size_t k = 0; k < loops; k++)
                {
                    uint8_t value = 0;

                    for (uint8_t parameter = 0; parameter < 2; parameter++)
                    {
                        if (((k * 2) + parameter) >= numberOfParameters)
                            break;

                        value <<= 4;
                        value |= defaultValue;
                    }

                    if (!write(startAddress, value, sectionParameterType_t::byte))
                        return false;

                    startAddress++;
                }
            }
            break;
            }
        }
    }

    return true;
}

///
/// \brief Checks for total memory usage of database.
/// \returns Database size in bytes.
///
uint32_t LESSDB::currentDBsize() const
{
    return memoryUsage;
}

///
/// \brief Checks for total amount of parameters stored in database.
/// \returns Number of parameters.
///
uint32_t LESSDB::currentDBparameters() const
{
    return memoryParameters;
}

///
/// \brief Retrieves maximum database size.
/// \returns Maximum database size in bytes.
///
uint32_t LESSDB::dbSize() const
{
    return storageAccess.size();
}

///
/// \brief Returns the database address at which last parameter is stored.
///
uint32_t LESSDB::lastParameterAddress() const
{
    return nextBlockAddress - 1;
}

///
/// \brief Returns first unused database address.
///
uint32_t LESSDB::nextParameterAddress() const
{
    return nextBlockAddress;
}

///
/// \brief Validates input parameters before attempting to read or write data.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterID     Parameter index.
/// \returns True if parameters are valid, false otherwise.
///
bool LESSDB::checkParameters(uint8_t blockID, uint8_t sectionID, size_t parameterIndex)
{
    // sanity check
    if (blockID >= blockCounter)
        return false;

    if (sectionID >= block[blockID].numberOfSections)
        return false;

    if (parameterIndex >= block[blockID].section[sectionID].numberOfParameters)
        return false;

    return true;
}

///
/// \brief Returns section address for specified section within block.
/// @param [in] blockID     Block index.
/// @param [in] sectionID   Section index.
/// \returns Section address.
///
uint32_t LESSDB::sectionAddress(uint8_t blockID, uint8_t sectionID)
{
    return initialAddress + block[blockID].address + block[blockID].section[sectionID].address;
}

///
/// \brief Used to specify start address in database.
/// By default, this address is set to 0.
/// @param [in] startAddress    New start address.
/// \returns True on success.
///
bool LESSDB::setStartAddress(uint32_t startAddress)
{
    if (startAddress >= storageAccess.size())
        return false;

    initialAddress = startAddress;
    return true;
}