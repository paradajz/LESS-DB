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

#include "DBMS.h"
#include <stdlib.h>

///
/// \brief Holds amount of blocks.
///
uint8_t blockCounter;

///
/// \brief Holds total memory usage for current database layout.
///
uint32_t memoryUsage;

///
/// \brief Pointer to array of DBMS blocks.
///
dbBlock_t *block;

///
/// \brief Function pointer used to read the memory contents.
///
bool (*readCallback)(uint32_t address, sectionParameterType_t type, int32_t &value);

///
/// \brief Function pointer used to update the memory contents.
///
bool (*writeCallback)(uint32_t address, int32_t value, sectionParameterType_t type);

///
/// \brief Returns section address for specified section within block.
/// @param [in] blockID     Block index.
/// @param [in] sectionID   Section index.
/// \returns Section address.
///
inline uint16_t getSectionAddress(uint8_t blockID, uint8_t sectionID)
{
    return block[blockID].address+block[blockID].section[sectionID].address;
};

///
/// \brief Default constructor
///
DBMS::DBMS()
{
    blockCounter = 0;
    memoryUsage = 0;
    block = NULL;
    readCallback = NULL;
    writeCallback = NULL;
}

///
/// \brief Calculates all addresses for specified blocks and sections.
/// @param [in] pointer         Pointer to database structure.
/// @param [in] numberOfBlocks  Total number of blocks in database structure.
/// \returns True on success, false otherwise.
///
bool DBMS::init(dbBlock_t *pointer, uint8_t numberOfBlocks)
{
    memoryUsage = 0;

    if ((pointer != NULL) && numberOfBlocks)
    {
        block = pointer;
        blockCounter = numberOfBlocks;

        for (int i=0; i<blockCounter; i++)
        {
            uint32_t blockUsage = 0;

            for (int j=0; j<block[i].numberOfSections; j++)
            {
                if (!j)
                {
                    //first section address is always 0
                    block[i].section[0].address = 0;
                }
                else
                {
                    switch(block[i].section[j-1].parameterType)
                    {
                        case BIT_PARAMETER:
                        block[i].section[j].address = ((block[i].section[j].numberOfParameters % 8 != 0) + block[i].section[j-1].numberOfParameters/8) + block[i].section[j-1].address;
                        break;

                        case BYTE_PARAMETER:
                        block[i].section[j].address  = block[i].section[j-1].numberOfParameters + block[i].section[j-1].address;
                        break;

                        case HALFBYTE_PARAMETER:
                        block[i].section[j].address  = block[i].section[j-1].numberOfParameters/2 + block[i].section[j-1].address;
                        break;

                        case WORD_PARAMETER:
                        block[i].section[j].address  = 2*block[i].section[j-1].numberOfParameters + block[i].section[j-1].address;
                        break;

                        default:
                        // case DWORD_PARAMETER:
                        block[i].section[j].address  = 4*block[i].section[j-1].numberOfParameters + block[i].section[j-1].address;
                        break;
                    }
                }
            }

            uint8_t lastSection = block[i].numberOfSections-1;

            switch(block[i].section[lastSection].parameterType)
            {
                case BIT_PARAMETER:
                blockUsage = block[i].section[lastSection].address+((block[i].section[lastSection].numberOfParameters%8 != 0) + block[i].section[lastSection].numberOfParameters/8);
                break;

                case BYTE_PARAMETER:
                blockUsage = block[i].section[lastSection].address + block[i].section[lastSection].numberOfParameters;
                break;

                case HALFBYTE_PARAMETER:
                blockUsage = block[i].section[lastSection].address + block[i].section[lastSection].numberOfParameters/2;
                break;

                case WORD_PARAMETER:
                blockUsage = block[i].section[lastSection].address + 2*block[i].section[lastSection].numberOfParameters;
                break;

                default:
                // case DWORD_PARAMETER:
                blockUsage = block[i].section[lastSection].address + 4*block[i].section[lastSection].numberOfParameters;
                break;
            }

            if (i < blockCounter-1)
                block[i+1].address = block[i].address + blockUsage;

            memoryUsage += blockUsage;

            if (memoryUsage >= LESSDB_SIZE)
                return false;
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
/// @param [in, out] value      Pointer to variable in which read value will be stored.
/// \returns True on success.
///
bool DBMS::read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t &value)
{
    if ((readCallback != NULL) && (block != NULL))
    {
        //sanity check
        if (!checkParameters(blockID, sectionID, parameterIndex))
            return false;

        uint16_t startAddress = getSectionAddress(blockID, sectionID);

        uint8_t arrayIndex;
        uint8_t bitIndex;

        switch(block[blockID].section[sectionID].parameterType)
        {
            case BIT_PARAMETER:
            arrayIndex = parameterIndex/8;
            bitIndex = parameterIndex - 8*arrayIndex;
            startAddress += arrayIndex;
            if (readCallback(startAddress, BIT_PARAMETER, value))
            {
                value = BIT_READ(value, bitIndex);
                return true;
            }
            break;

            case BYTE_PARAMETER:
            startAddress += parameterIndex;
            if (readCallback(startAddress, BYTE_PARAMETER, value))
            {
                //sanitize
                value &= (int32_t)0xFF;
                return true;
            }
            break;

            case HALFBYTE_PARAMETER:
            startAddress += parameterIndex/2;
            if (readCallback(startAddress, HALFBYTE_PARAMETER, value))
            {
                if (parameterIndex%2)
                {
                    value >>= 4;
                }

                //sanitize
                value &= (int32_t)0x0F;
                return true;
            }
            break;

            case WORD_PARAMETER:
            startAddress += parameterIndex*2;
            if (readCallback(startAddress, WORD_PARAMETER, value))
            {
                //sanitize
                value &= (int32_t)0xFFFF;
                return true;
            }
            break;

            default:
            // case DWORD_PARAMETER:
            startAddress += parameterIndex*4;
            return readCallback(startAddress, DWORD_PARAMETER, value);
            break;
        }
    }

    return false;
}

///
/// \brief Reads a value from database without error checking.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// \returns Value from database.
///
int32_t DBMS::read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex)
{
    int32_t value = -1;

    read(blockID, sectionID, parameterIndex, value);

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
bool DBMS::update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue)
{
    if ((writeCallback != NULL) && (readCallback != NULL) && (block != NULL))
    {
        //sanity check
        if (!checkParameters(blockID, sectionID, parameterIndex))
            return false;

        uint16_t startAddress = getSectionAddress(blockID, sectionID);
        uint8_t parameterType = block[blockID].section[sectionID].parameterType;

        uint8_t arrayIndex;
        int32_t arrayValue;
        uint8_t bitIndex;
        int32_t checkValue;

        switch(parameterType)
        {
            case BIT_PARAMETER:
            //sanitize input
            newValue &= (int32_t)0x01;
            arrayIndex = parameterIndex/8;
            bitIndex = parameterIndex - 8*arrayIndex;
            startAddress += arrayIndex;

            //read existing value first
            if (readCallback(startAddress, BIT_PARAMETER, arrayValue))
            {
                //update value with new bit
                BIT_WRITE(arrayValue, bitIndex, newValue);

                //write to memory
                if (writeCallback(startAddress, arrayValue, BIT_PARAMETER))
                {
                    if (readCallback(startAddress, BIT_PARAMETER, checkValue))
                    {
                        return (arrayValue == checkValue);
                    }
                }
            }
            break;

            case BYTE_PARAMETER:
            //sanitize input
            newValue &= (int32_t)0xFF;
            startAddress += parameterIndex;
            if (writeCallback(startAddress, newValue, BYTE_PARAMETER))
            {
                if (readCallback(startAddress, BYTE_PARAMETER, checkValue))
                {
                    return (newValue == checkValue);
                }
            }
            break;

            case HALFBYTE_PARAMETER:
            //sanitize input
            newValue &= (int32_t)0x0F;
            startAddress += (parameterIndex/2);
            //read old value first
            if (readCallback(startAddress, HALFBYTE_PARAMETER, arrayValue))
            {
                if (parameterIndex % 2)
                {
                    //clear content in bits 4-7 and update the value
                    arrayValue &= 0x0F;
                    arrayValue |= (newValue << 4);
                }
                else
                {
                    //clear content in bits 0-3 and update the value
                    arrayValue &= 0xF0;
                    arrayValue |= newValue;
                }

                if (writeCallback(startAddress, arrayValue, HALFBYTE_PARAMETER))
                {
                    if (readCallback(startAddress, HALFBYTE_PARAMETER, checkValue))
                    {
                        return (arrayValue == checkValue);
                    }
                }
            }
            break;

            case WORD_PARAMETER:
            //sanitize input
            newValue &= (int32_t)0xFFFF;
            startAddress += (parameterIndex*2);
            if (writeCallback(startAddress, newValue, WORD_PARAMETER))
            {
                if (readCallback(startAddress, WORD_PARAMETER, checkValue))
                {
                    return (newValue == checkValue);
                }
            }
            break;

            case DWORD_PARAMETER:
            startAddress += (parameterIndex*4);
            if (writeCallback(startAddress, newValue, DWORD_PARAMETER))
            {
                if (readCallback(startAddress, DWORD_PARAMETER, checkValue))
                {
                    return (newValue == checkValue);
                }
            }
            break;
        }
    }

    return false;
}

///
/// \brief Clears entire memory by writing 0xFF to each location.
///
void DBMS::clear()
{
    if (writeCallback != NULL)
    {
        for (int i=0; i<LESSDB_SIZE; i++)
            writeCallback(i, 0x00, BYTE_PARAMETER);
    }
}

///
/// \brief Writes default values to memory from defaultValue parameter.
/// @param [in] type    Type of initialization (initPartial or initWipe). initWipe will simply overwrite currently existing
///                     data. initPartial will leave data as is, but only if preserveOnPartialReset parameter is set to true.
///
void DBMS::initData(initType_t type)
{
    for (int i=0; i<blockCounter; i++)
    {
        for (int j=0; j<block[i].numberOfSections; j++)
        {
            if (block[i].section[j].preserveOnPartialReset && (type == initPartial))
                continue;

            uint8_t parameterType = block[i].section[j].parameterType;
            uint32_t defaultValue = block[i].section[j].defaultValue;
            int16_t numberOfParameters = block[i].section[j].numberOfParameters;

            switch(parameterType)
            {
                case BYTE_PARAMETER:
                case WORD_PARAMETER:
                case DWORD_PARAMETER:
                for (int k=0; k<numberOfParameters; k++)
                {
                    if (block[i].section[j].autoIncrement)
                        update(i, j, k, defaultValue+k);
                    else
                        update(i, j, k, defaultValue);
                }
                break;

                case BIT_PARAMETER:
                case HALFBYTE_PARAMETER:
                //no auto-increment here
                for (int k=0; k<numberOfParameters; k++)
                {
                    update(i, j, k, defaultValue);
                }
                break;
            }
        }
    }
}

///
/// \brief Checks for total memory usage of database.
/// \returns Database size in bytes.
///
uint32_t DBMS::getDBsize()
{
    return memoryUsage;
}

///
/// \brief Validates input parameters before attempting to read or write data.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterID     Parameter index.
/// \returns True if parameters are valid, false otherwise.
///
bool DBMS::checkParameters(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex)
{
    //sanity check
    if (blockID >= blockCounter)
    {
        return false;
    }

    if (sectionID >= block[blockID].numberOfSections)
    {
        return false;
    }

    if (parameterIndex >= (uint16_t)block[blockID].section[sectionID].numberOfParameters)
    {
        return false;
    }

    return true;
}

///
/// \brief Handler used to define callback function for memory read requests.
///
void DBMS::setHandleRead(bool(*fptr)(uint32_t address, sectionParameterType_t type, int32_t &value))
{
    readCallback = fptr;
}

///
/// \brief Handler used to define callback function for memory write requests.
///
void DBMS::setHandleWrite(bool(*fptr)(uint32_t address, int32_t value, sectionParameterType_t type))
{
    writeCallback = fptr;
}
