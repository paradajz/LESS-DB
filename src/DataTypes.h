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
/// \ingroup avrDBMS
/// @{

///
/// \brief List of possible parameter types in section.
///
typedef enum
{
    BIT_PARAMETER,
    BYTE_PARAMETER,
    WORD_PARAMETER,
    DWORD_PARAMETER
} sectionParameterType_t;

///
/// \brief A structure holding information for a single section.
///
typedef struct
{
    uint16_t numberOfParameters;
    sectionParameterType_t parameterType;
    bool preserveOnPartialReset;
    uint16_t defaultValue;
    bool autoIncrement;
} dbSection_t;

///
/// \brief A structure holding information for a single block.
///
typedef struct
{
    uint8_t numberOfSections;
    uint16_t blockStartAddress;
    uint16_t sectionAddress[DBMS_MAX_SECTIONS];
    dbSection_t section[DBMS_MAX_SECTIONS];
} dbBlock_t;

///
/// \brief List of possible database initialization types.
///
typedef enum
{
    initPartial,
    initWipe
} initType_t;

/// @}