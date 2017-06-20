#pragma once

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
    uint8_t sections;
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