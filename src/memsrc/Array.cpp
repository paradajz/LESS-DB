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

#include "MemRead.h"

uint8_t memoryArray[EEPROM_SIZE];

int32_t memoryRead(uint32_t address, sectionParameterType_t type)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        if (address >= EEPROM_SIZE)
        {
            return -1;
        }
        else
        {
            return memoryArray[address];
        }
        break;

        case WORD_PARAMETER:
        if (address >= (EEPROM_SIZE-2))
        {
            return -1;
        }
        else
        {
            uint16_t value = memoryArray[address+1];
            value <<= 8;
            value |= memoryArray[address+0];
            return value;
        }
        break;

        default:
        // case DWORD_PARAMETER:
        if (address >= (EEPROM_SIZE-4))
        {
            return -1;
        }
        else
        {
            uint32_t value = memoryArray[address+3];
            value <<= 8;
            value |= memoryArray[address+2];
            value <<= 8;
            value |= memoryArray[address+1];
            value <<= 8;
            value |= memoryArray[address+0];
            return value;
        }
        break;
    }
}

bool memoryWrite(uint32_t address, int32_t value, sectionParameterType_t type)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        if (address >= EEPROM_SIZE)
        {
            return false;
        }
        else
        {
            memoryArray[address] = value;
        }
        break;

        case WORD_PARAMETER:
        if (address >= (EEPROM_SIZE-2))
        {
            return false;
        }
        else
        {
            memoryArray[address+0] = (value >> 0) & (uint16_t)0xFF;
            memoryArray[address+1] = (value >> 8) & (uint16_t)0xFF;
        }
        break;

        case DWORD_PARAMETER:
        if (address >= (EEPROM_SIZE-4))
        {
            return false;
        }
        else
        {
            memoryArray[address+0] = (value >> 0) & (uint32_t)0xFF;
            memoryArray[address+1] = (value >> 8) & (uint32_t)0xFF;
            memoryArray[address+2] = (value >> 16) & (uint32_t)0xFF;
            memoryArray[address+3] = (value >> 24) & (uint32_t)0xFF;
        }
        break;
    }

    return true;
}