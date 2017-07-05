#pragma once

#include "DataTypes.h"

#ifndef EEPROM_SIZE
#error EEPROM size not defined
#endif

#ifndef DBMS_MAX_BLOCKS
#error Number of DBMS blocks undefined
#endif

#ifndef DBMS_MAX_SECTIONS
#error Number of DBMS sections undefined
#endif

#ifdef DBMS_ENABLE_ASYNC_UPDATE
#ifndef DBMS_UPDATE_BUFFER_SIZE
#error DBMS update buffer size undefined
#endif
#endif

///
/// \brief Simple database-like management system.
/// \ingroup eeprom
/// @{
///

class DBMS
{
    public:
    DBMS();
    void init();
    #ifdef DBMS_ENABLE_ASYNC_UPDATE
    bool checkQueue();
    #endif
    void clear();
    int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterID = 0);
    bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterID, int32_t newValue, bool async = false);
    uint32_t getDBsize();

    protected:
    bool addBlock();
    bool addBlocks(uint8_t numberOfBlocks);
    bool addSection(uint8_t blockID, dbSection_t section);
    void commitLayout();
    void initData(initType_t type = initWipe);

    private:
    #ifdef DBMS_ENABLE_ASYNC_UPDATE
    void queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType);
    #endif

    #ifdef DBMS_ENABLE_ASYNC_UPDATE
    ///
    /// \brief Variables used for internal buffer implementation used for async EEPROM update.
    /// @{
    ///
    uint8_t     eeprom_update_bufer_param_type[DBMS_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_value[DBMS_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_address[DBMS_UPDATE_BUFFER_SIZE];
    uint8_t     eeprom_update_buffer_head;
    uint8_t     eeprom_update_buffer_tail;
    /// @}
    #endif

    uint8_t blockCounter;
    uint32_t memoryUsage;
};

/// @}