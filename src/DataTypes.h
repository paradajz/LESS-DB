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

#pragma once

#include <inttypes.h>

///
/// \brief List of possible parameter types in section.
///
typedef enum
{
    BIT_PARAMETER,
    BYTE_PARAMETER,
    HALFBYTE_PARAMETER,
    WORD_PARAMETER,
    DWORD_PARAMETER,
    INVALID_PARAMETER
} sectionParameterType_t;

///
/// \brief A structure holding information for a single section.
///
typedef struct
{
    int16_t numberOfParameters;
    sectionParameterType_t parameterType;
    bool preserveOnPartialReset;
    uint16_t defaultValue;
    bool autoIncrement;
    int16_t address;
} dbSection_t;

///
/// \brief A structure holding information for a single block.
///
typedef struct
{
    int16_t address;
    int8_t numberOfSections;
    dbSection_t *section;
} dbBlock_t;

///
/// \brief List of possible database initialization types.
///
typedef enum
{
    initPartial,
    initFull
} initType_t;
