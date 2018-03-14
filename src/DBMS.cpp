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
#include "memsrc/MemRead.h"

///
/// \brief Array of DBMS blocks.
/// \ingroup avrDBMS
///
static dbBlock_t block[DBMS_MAX_BLOCKS];

///
/// \brief Holds amount of blocks.
///
static uint8_t blockCounter;

///
/// \brief Holds total memory usage in EEPROM for current database layout.
///
static uint32_t memoryUsage;

///
/// \brief Returns section address for specified section within block.
/// @param [in] blockID     Block index.
/// @param [in] sectionID   Section index.
/// \returns Section address.
/// \ingroup avrDBMS
///
inline uint16_t getSectionAddress(uint8_t blockID, uint8_t sectionID)
{
    return block[blockID].blockStartAddress+block[blockID].sectionAddress[sectionID];
};

///
/// \brief Default constructor
///
DBMS::DBMS()
{

}

void DBMS::init()
{
    for (int i=0; i<DBMS_MAX_BLOCKS; i++)
    {
        block[i].numberOfSections = 0;
        block[i].blockStartAddress = 0;

        for (int j=0; j<DBMS_MAX_SECTIONS; j++)
        {
            block[i].sectionAddress[j] = 0;
            block[i].section[j].numberOfParameters = -1;
            block[i].section[j].parameterType = INVALID_PARAMETER;
            block[i].section[j].defaultValue = 0;
        }
    }

    blockCounter = 0;
    memoryUsage = 0;
}

///
/// \brief Reads a value from database.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// \returns Retrieved value.
///
bool DBMS::read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t &value)
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
        if (memoryRead(startAddress, BIT_PARAMETER, value))
        {
            value = BIT_READ(value, bitIndex);
            return true;
        }
        break;

        case BYTE_PARAMETER:
        startAddress += parameterIndex;
        return memoryRead(startAddress, BYTE_PARAMETER, value);
        break;

        case HALFBYTE_PARAMETER:
        startAddress += (parameterIndex/2);
        if (memoryRead(startAddress, HALFBYTE_PARAMETER, value))
        {
            value = (parameterIndex%2) ? (arrayIndex >> 4) : (arrayIndex & 0x0F);
            return true;
        }
        break;

        case WORD_PARAMETER:
        startAddress += ((uint16_t)parameterIndex*2);
        return memoryRead(startAddress, WORD_PARAMETER, value);
        break;

        default:
        // case DWORD_PARAMETER:
        startAddress += ((uint16_t)parameterIndex*4);
        return memoryRead(startAddress, DWORD_PARAMETER, value);
        break;
    }

    return false;
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
        arrayIndex = parameterIndex/8;
        bitIndex = parameterIndex - 8*arrayIndex;

        if (memoryRead(startAddress+arrayIndex, BIT_PARAMETER, arrayValue))
        {
            BIT_WRITE(arrayValue, bitIndex, newValue);

            if (memoryWrite(startAddress+arrayIndex, arrayValue, BIT_PARAMETER))
            {
                if (memoryRead(startAddress+arrayIndex, BIT_PARAMETER, checkValue))
                {
                    return (arrayValue == checkValue);
                }
            }
        }
        break;

        case BYTE_PARAMETER:
        if (memoryWrite(startAddress+parameterIndex, newValue, BYTE_PARAMETER))
        {
            if (memoryRead(startAddress+parameterIndex, BYTE_PARAMETER, checkValue))
            {
                return (newValue == checkValue);
            }
        }
        break;

        case HALFBYTE_PARAMETER:
        //sanitize input
        newValue &= 0x0F;
        //read old value first
        if (memoryRead(startAddress+(parameterIndex/2), HALFBYTE_PARAMETER, arrayValue))
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

            if (memoryWrite(startAddress+(parameterIndex/2), arrayValue, HALFBYTE_PARAMETER))
            {
                if (memoryRead(startAddress+(parameterIndex/2), HALFBYTE_PARAMETER, checkValue))
                {
                    return (arrayValue == checkValue);
                }
            }
        }
        break;

        case WORD_PARAMETER:
        if (memoryWrite(startAddress+(parameterIndex*2), newValue, WORD_PARAMETER))
        {
            if (memoryRead(startAddress+parameterIndex, WORD_PARAMETER, checkValue))
            {
                return (newValue == checkValue);
            }
        }
        break;

        case DWORD_PARAMETER:
        if (memoryWrite(startAddress+(parameterIndex*4), newValue, DWORD_PARAMETER))
        {
            if (memoryRead(startAddress+parameterIndex, DWORD_PARAMETER, checkValue))
            {
                return (newValue == checkValue);
            }
        }
        break;
    }

    return false;
}

///
/// \brief Clears entire EEPROM memory by writing 0xFF to each location.
///
void DBMS::clear()
{
    for (int i=0; i<EEPROM_SIZE; i++)
        memoryWrite(i, 0xFF, BYTE_PARAMETER);
}

///
/// \brief Adds specified number of blocks to current layout.
/// @param [in] numberOfBlocks  Number of blocks to add.
/// \returns True on success, false otherwise.
///
bool DBMS::addBlocks(uint8_t numberOfBlocks)
{
    if (blockCounter+numberOfBlocks > DBMS_MAX_BLOCKS)
        return false;

    blockCounter += numberOfBlocks;

    return true;
}

///
/// \brief Adds section to specified block.
/// @param [in] blockID     Block on which to add section.
/// @param [in] section     Structure holding description of section.
/// \returns True on success, false otherwise.
///
bool DBMS::addSection(uint8_t blockID, dbSection_t section)
{
    if (block[blockID].numberOfSections > DBMS_MAX_SECTIONS)
        return false;

    block[blockID].section[block[blockID].numberOfSections].parameterType = section.parameterType;
    block[blockID].section[block[blockID].numberOfSections].preserveOnPartialReset = section.preserveOnPartialReset;
    block[blockID].section[block[blockID].numberOfSections].defaultValue = section.defaultValue;
    block[blockID].section[block[blockID].numberOfSections].numberOfParameters = section.numberOfParameters;
    block[blockID].section[block[blockID].numberOfSections].autoIncrement = section.autoIncrement;

    block[blockID].numberOfSections++;

    return true;
}

///
/// \brief Calculates all addresses for specified blocks and sections.
///
void DBMS::commitLayout()
{
    memoryUsage = 0;

    for (int i=0; i<blockCounter; i++)
    {
        uint32_t blockUsage = 0;

        for (int j=0; j<block[i].numberOfSections; j++)
        {
            if (!j)
            {
                //first section address is always 0
                block[i].sectionAddress[0] = 0;
            }
            else
            {
                switch(block[i].section[j-1].parameterType)
                {
                    case BIT_PARAMETER:
                    block[i].sectionAddress[j] = ((block[i].section[j].numberOfParameters % 8 != 0) + block[i].section[j-1].numberOfParameters/8) + block[i].sectionAddress[j-1];
                    break;

                    case BYTE_PARAMETER:
                    block[i].sectionAddress[j] = block[i].section[j-1].numberOfParameters + block[i].sectionAddress[j-1];
                    break;

                    case HALFBYTE_PARAMETER:
                    block[i].sectionAddress[j] = block[i].section[j-1].numberOfParameters/2 + block[i].sectionAddress[j-1];
                    break;

                    case WORD_PARAMETER:
                    block[i].sectionAddress[j] = 2*block[i].section[j-1].numberOfParameters + block[i].sectionAddress[j-1];
                    break;

                    case DWORD_PARAMETER:
                    block[i].sectionAddress[j] = 4*block[i].section[j-1].numberOfParameters + block[i].sectionAddress[j-1];
                    break;
                }
            }
        }

        uint8_t lastSection = block[i].numberOfSections-1;

        switch(block[i].section[lastSection].parameterType)
        {
            case BIT_PARAMETER:
            blockUsage = block[i].sectionAddress[lastSection]+((block[i].section[lastSection].numberOfParameters%8 != 0) + block[i].section[lastSection].numberOfParameters/8);
            break;

            case BYTE_PARAMETER:
            blockUsage = block[i].sectionAddress[lastSection] + block[i].section[lastSection].numberOfParameters;
            break;

            case HALFBYTE_PARAMETER:
            blockUsage = block[i].sectionAddress[lastSection] + block[i].section[lastSection].numberOfParameters/2;
            break;

            case WORD_PARAMETER:
            blockUsage = block[i].sectionAddress[lastSection] + 2*block[i].section[lastSection].numberOfParameters;
            break;

            case DWORD_PARAMETER:
            blockUsage = block[i].sectionAddress[lastSection] + 4*block[i].section[lastSection].numberOfParameters;
            break;
        }

        if (i < blockCounter-1)
            block[i+1].blockStartAddress = block[i].blockStartAddress + blockUsage;

        memoryUsage += blockUsage;
    }
}

///
/// \brief Writes default values to EEPROM from defaultValue parameter.
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

    if (parameterIndex >= block[blockID].section[sectionID].numberOfParameters)
    {
        return false;
    }

    return true;
}