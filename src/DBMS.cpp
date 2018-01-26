/*
    Copyright 2017 Igor Petrovi�

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
#include <avr/eeprom.h>

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
    #ifdef DBMS_ENABLE_ASYNC_UPDATE
    for (int i=0; i<DBMS_UPDATE_BUFFER_SIZE; i++)
    {
        eeprom_update_bufer_param_type[i] = 0;
        eeprom_update_bufer_value[i] = 0;
        eeprom_update_bufer_address[i] = 0;
    }

    eeprom_update_buffer_head = 0;
    eeprom_update_buffer_tail = 0;
    #endif
}

///
/// \brief Reads a value from database.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// \returns Retrieved value.
///
int32_t DBMS::read(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex)
{
    uint16_t startAddress = getSectionAddress(blockID, sectionID);
    uint8_t parameterType = block[blockID].section[sectionID].parameterType;

    uint8_t arrayIndex;
    uint8_t bitIndex;

    switch(parameterType)
    {
        case BIT_PARAMETER:
        arrayIndex = parameterIndex/8;
        bitIndex = parameterIndex - 8*arrayIndex;
        startAddress += arrayIndex;
        return BIT_READ(eeprom_read_byte((uint8_t*)startAddress), bitIndex);
        break;

        case BYTE_PARAMETER:
        startAddress += parameterIndex;
        return eeprom_read_byte((uint8_t*)startAddress);
        break;

        case WORD_PARAMETER:
        startAddress += ((uint16_t)parameterIndex*2);
        return eeprom_read_word((uint16_t*)startAddress);
        break;

        case DWORD_PARAMETER:
        startAddress += ((uint16_t)parameterIndex*4);
        return eeprom_read_dword((uint32_t*)startAddress);
        break;
    }

    return 0;
}

///
/// \brief Updates value for specified block and section in database.
/// @param [in] blockID         Block index.
/// @param [in] sectionID       Section index.
/// @param [in] parameterIndex  Parameter index.
/// @param [in] newValue        New value for parameter.
/// @param [in] async           Whether to update value immediately (false) or later (true).
/// \returns True on success, false otherwise.
///
bool DBMS::update(uint8_t blockID, uint8_t sectionID, uint16_t parameterIndex, int32_t newValue, bool async)
{
    uint16_t startAddress = getSectionAddress(blockID, sectionID);

    if (startAddress > EEPROM_SIZE)
        return 0;

    uint8_t parameterType = block[blockID].section[sectionID].parameterType;

    uint8_t arrayIndex;
    uint8_t arrayValue;
    uint8_t bitIndex;

    switch(parameterType)
    {
        case BIT_PARAMETER:
        arrayIndex = parameterIndex/8;
        bitIndex = parameterIndex - 8*arrayIndex;
        arrayValue = eeprom_read_byte((uint8_t*)startAddress+arrayIndex);
        BIT_WRITE(arrayValue, bitIndex, newValue);
        #ifdef DBMS_ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+arrayIndex, arrayValue, BIT_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
            return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));
        }
        #else
        eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
        return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));
        #endif
        break;

        case BYTE_PARAMETER:
        #ifdef DBMS_ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+parameterIndex, newValue, BYTE_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_byte((uint8_t*)startAddress+parameterIndex, newValue);
            return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterIndex));
        }
        #else
        eeprom_update_byte((uint8_t*)startAddress+parameterIndex, newValue);
        return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterIndex));
        #endif
        break;

        case WORD_PARAMETER:
        #ifdef DBMS_ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+parameterIndex, newValue, WORD_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_word((uint16_t*)startAddress+parameterIndex, newValue);
            return (newValue == read(blockID, sectionID, parameterIndex));
        }
        #else
        eeprom_update_word((uint16_t*)startAddress+parameterIndex, newValue);
        return (newValue == read(blockID, sectionID, parameterIndex));
        #endif
        break;

        case DWORD_PARAMETER:
        #ifdef DBMS_ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+parameterIndex, newValue, DWORD_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_dword((uint32_t*)startAddress+parameterIndex, newValue);
            return (newValue == read(blockID, sectionID, parameterIndex));
        }
        #else
        eeprom_update_dword((uint32_t*)startAddress+parameterIndex, newValue);
        return (newValue == read(blockID, sectionID, parameterIndex));
        #endif
        break;
    }

    return 0;
}

///
/// \brief Clears entire EEPROM memory by writing 0xFF to each location.
///
void DBMS::clear()
{
    for (int i=0; i<EEPROM_SIZE; i++)
        eeprom_update_byte((uint8_t*)i, 0xFF);
}

///
/// \brief Adds single block to current layout.
/// \returns True on success, false otherwise.
///
bool DBMS::addBlock()
{
    if (blockCounter >= DBMS_MAX_BLOCKS)
        return false;

    blockCounter++;

    return true;
}

///
/// \brief Adds specified number of blocks to current layout.
/// @param [in] numberOfBlocks  Number of blocks to add.
/// \returns True on success, false otherwise.
///
bool DBMS::addBlocks(uint8_t numberOfBlocks)
{
    if (blockCounter+numberOfBlocks >= DBMS_MAX_BLOCKS)
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
    if (block[blockID].numberOfSections >= DBMS_MAX_SECTIONS)
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

            uint16_t startAddress = getSectionAddress(i, j);
            uint8_t parameterType = block[i].section[j].parameterType;
            uint32_t defaultValue = block[i].section[j].defaultValue;
            uint16_t numberOfParameters = block[i].section[j].numberOfParameters;

            switch(parameterType)
            {
                case BIT_PARAMETER:
                for (int k=0; k<(int)(numberOfParameters/8+1); k++)
                    eeprom_update_byte((uint8_t*)startAddress+k, defaultValue);
                break;

                case BYTE_PARAMETER:
                while (numberOfParameters--)
                {
                    if (block[i].section[j].autoIncrement)
                        eeprom_update_byte((uint8_t*)startAddress+numberOfParameters, numberOfParameters+defaultValue);
                    else
                        eeprom_update_byte((uint8_t*)startAddress+numberOfParameters, defaultValue);
                }
                break;

                case WORD_PARAMETER:
                while (numberOfParameters--)
                {
                    if (block[i].section[j].autoIncrement)
                        eeprom_update_word((uint16_t*)(uint16_t)(startAddress+(numberOfParameters*2)), numberOfParameters+defaultValue);
                    else
                        eeprom_update_word((uint16_t*)(uint16_t)(startAddress+(numberOfParameters*2)), (uint16_t)defaultValue);
                }
                break;

                case DWORD_PARAMETER:
                while (numberOfParameters--)
                {
                    if (block[i].section[j].autoIncrement)
                        eeprom_update_dword((uint32_t*)(uint16_t)(startAddress+(numberOfParameters*4)), (uint32_t)numberOfParameters+defaultValue);
                    else
                        eeprom_update_dword((uint32_t*)(uint16_t)(startAddress+(numberOfParameters*4)), (uint32_t)defaultValue);
                }
                break;
            }
        }
    }
}

#if defined(DBMS_ENABLE_ASYNC_UPDATE) || defined(__DOXYGEN__)
///
/// \brief Writes data to internal queue instead of directly to EEPROM.
/// @param [in] eepromAddress   Address at which to write data.
/// @param [in] data            Data to write.
/// @param [in] parameterType   Type of parameter.
///
void DBMS::queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType)
{
    uint8_t index = eeprom_update_buffer_head + 1;

    if (index >= DBMS_UPDATE_BUFFER_SIZE)
        index = 0;

    //if buffer is full, wait until there is some space
    if (eeprom_update_buffer_tail == index)
    {
        while (!checkQueue());
    }

    eeprom_update_bufer_param_type[index] = parameterType;
    eeprom_update_bufer_value[index] = data;
    eeprom_update_bufer_address[index] = eepromAddress;
    eeprom_update_buffer_head = index;
}

///
/// \brief Checks if there is any data in queue.
/// If there is data, a single write command will be executed, removing one queued event from queue.
/// \return True if there is data in queue, false otherwise.
///
bool DBMS::checkQueue()
{
    //write queued data to eeprom

    if (eeprom_update_buffer_head == eeprom_update_buffer_tail)
    {
        //buffer is empty
        return false;
    }

    //there is something in buffer
    uint8_t index = eeprom_update_buffer_tail + 1;

    if (index >= DBMS_UPDATE_BUFFER_SIZE)
        index = 0;

    //write
    switch(eeprom_update_bufer_param_type[index])
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        eeprom_update_byte((uint8_t*)eeprom_update_bufer_address[index], eeprom_update_bufer_value[index]);
        break;

        case WORD_PARAMETER:
        eeprom_update_word((uint16_t*)eeprom_update_bufer_address[index], eeprom_update_bufer_value[index]);
        break;
    }

    eeprom_update_buffer_tail = index;

    return true;
}
#endif

///
/// \brief Checks for total memory usage of database.
/// \returns Database size in bytes.
///
uint32_t DBMS::getDBsize()
{
    return memoryUsage;
}