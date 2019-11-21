#include "unity/src/unity.h"
#include "unity/Helpers.h"
#include "LESSDB.h"

#define NUMBER_OF_BLOCKS 6
#define NUMBER_OF_SECTIONS 6
#define TEST_BLOCK_INDEX 0
#define LESSDB_SIZE 1021

namespace
{
    uint8_t memoryArray[LESSDB_SIZE];

    const size_t sectionParams[NUMBER_OF_SECTIONS] = { 5, 10, 15, 10, 15, 10 };

    const LESSDB::sectionParameterType_t sectionTypes[NUMBER_OF_SECTIONS] = {
        LESSDB::sectionParameterType_t::bit,
        LESSDB::sectionParameterType_t::byte,
        LESSDB::sectionParameterType_t::halfByte,
        LESSDB::sectionParameterType_t::word,
        LESSDB::sectionParameterType_t::dword,
        LESSDB::sectionParameterType_t::byte
    };

    const uint16_t defaultValues[NUMBER_OF_SECTIONS] = { 1, 10, 15, 20, 25, 30 };

    LESSDB::section_t block0sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[1],
          .autoIncrement = true,
          .address = 0 },

        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::section_t block1sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[1],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::section_t block2sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[1],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::section_t block3sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[1],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::section_t block4sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[1],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::section_t block5sections[NUMBER_OF_SECTIONS] = {
        { .numberOfParameters = sectionParams[5],
          .parameterType = sectionTypes[5],
          .preserveOnPartialReset = false,
          .defaultValue = defaultValues[5],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[0],
          .parameterType = sectionTypes[0],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[0],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[1],
          .parameterType = sectionTypes[1],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[1],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[2],
          .parameterType = sectionTypes[2],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[2],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[3],
          .parameterType = sectionTypes[3],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[3],
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = sectionParams[4],
          .parameterType = sectionTypes[4],
          .preserveOnPartialReset = true,
          .defaultValue = defaultValues[4],
          .autoIncrement = false,
          .address = 0 }
    };

    LESSDB::block_t dbLayout[NUMBER_OF_BLOCKS] = {
        // block 0
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block0sections,
            .address = 0,
        },

        // block 1
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block1sections,
            .address = 0,
        },

        // block 2
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block2sections,
            .address = 0,
        },

        // block 3
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block3sections,
            .address = 0,
        },

        // block 4
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block4sections,
            .address = 0,
        },

        // block 5
        {
            .numberOfSections = NUMBER_OF_SECTIONS,
            .section = block5sections,
            .address = 0,
        }
    };

    bool memoryReadFail(uint32_t address, LESSDB::sectionParameterType_t type, int32_t& value)
    {
        return false;
    }

    bool memoryWriteFail(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type)
    {
        return false;
    }

    bool memoryRead(uint32_t address, LESSDB::sectionParameterType_t type, int32_t& value)
    {
        switch (type)
        {
        case LESSDB::sectionParameterType_t::bit:
        case LESSDB::sectionParameterType_t::byte:
        case LESSDB::sectionParameterType_t::halfByte:
            value = memoryArray[address];
            break;

        case LESSDB::sectionParameterType_t::word:
            value = memoryArray[address + 1];
            value <<= 8;
            value |= memoryArray[address + 0];
            break;

        default:
            // case LESSDB::sectionParameterType_t::dword:
            value = memoryArray[address + 3];
            value <<= 8;
            value |= memoryArray[address + 2];
            value <<= 8;
            value |= memoryArray[address + 1];
            value <<= 8;
            value |= memoryArray[address + 0];
            break;
        }

        return true;
    }

    bool memoryWrite(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type)
    {
        switch (type)
        {
        case LESSDB::sectionParameterType_t::bit:
        case LESSDB::sectionParameterType_t::byte:
        case LESSDB::sectionParameterType_t::halfByte:
            memoryArray[address] = value;
            break;

        case LESSDB::sectionParameterType_t::word:
            memoryArray[address + 0] = (value >> 0) & (uint16_t)0xFF;
            memoryArray[address + 1] = (value >> 8) & (uint16_t)0xFF;
            break;

        case LESSDB::sectionParameterType_t::dword:
            memoryArray[address + 0] = (value >> 0) & (uint32_t)0xFF;
            memoryArray[address + 1] = (value >> 8) & (uint32_t)0xFF;
            memoryArray[address + 2] = (value >> 16) & (uint32_t)0xFF;
            memoryArray[address + 3] = (value >> 24) & (uint32_t)0xFF;
            break;
        }

        return true;
    }
}    // namespace

TEST_CASE(Read)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    int32_t value;

    // bit section
    for (int i = 0; i < sectionParams[0]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 0, i, value) == true);
        TEST_ASSERT(value == defaultValues[0]);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < sectionParams[1]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 1, i, value) == true);
        TEST_ASSERT(value == defaultValues[1] + i);
    }

    // half-byte section
    for (int i = 0; i < sectionParams[2]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 2, i, value) == true);
        TEST_ASSERT(value == defaultValues[2]);
    }

    // word section
    for (int i = 0; i < sectionParams[3]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 3, i, value) == true);
        TEST_ASSERT(value == defaultValues[3]);
    }

    // dword section
    for (int i = 0; i < sectionParams[4]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 4, i, value) == true);
        TEST_ASSERT(value == defaultValues[4]);
    }

    // try reading directly
    value = db.read(TEST_BLOCK_INDEX, 1, 0);
    TEST_ASSERT(value == defaultValues[1]);

    // perform the same round of tests with different starting point
    LESSDB db2(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db2.setStartAddress(100) == true);
    TEST_ASSERT(db2.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db2.initData(LESSDB::factoryResetType_t::full);

    // bit section
    for (int i = 0; i < sectionParams[0]; i++)
    {
        TEST_ASSERT(db2.read(TEST_BLOCK_INDEX, 0, i, value) == true);
        TEST_ASSERT(value == defaultValues[0]);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < sectionParams[1]; i++)
    {
        TEST_ASSERT(db2.read(TEST_BLOCK_INDEX, 1, i, value) == true);
        TEST_ASSERT(value == defaultValues[1] + i);
    }

    // half-byte section
    for (int i = 0; i < sectionParams[2]; i++)
    {
        TEST_ASSERT(db2.read(TEST_BLOCK_INDEX, 2, i, value) == true);
        TEST_ASSERT(value == defaultValues[2]);
    }

    // word section
    for (int i = 0; i < sectionParams[3]; i++)
    {
        TEST_ASSERT(db2.read(TEST_BLOCK_INDEX, 3, i, value) == true);
        TEST_ASSERT(value == defaultValues[3]);
    }

    // dword section
    for (int i = 0; i < sectionParams[4]; i++)
    {
        TEST_ASSERT(db2.read(TEST_BLOCK_INDEX, 4, i, value) == true);
        TEST_ASSERT(value == defaultValues[4]);
    }

    // try reading directly
    value = db2.read(TEST_BLOCK_INDEX, 1, 0);
    TEST_ASSERT(value == defaultValues[1]);

    // try setting invalid address
    TEST_ASSERT(db2.setStartAddress(LESSDB_SIZE) == false);
}

TEST_CASE(Update)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    int32_t value;
    bool    returnValue;

    // section 0, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 0, 1);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 1);

    // section 0, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 1, 0);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 0);

    // section 1, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 0, 240);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    TEST_ASSERT(value == 240);

    // section 1, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 1, 143);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    TEST_ASSERT(value == 143);

    // section 2, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 0, 4);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    TEST_ASSERT(value == 4);

    // section 2, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 1, 12);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    TEST_ASSERT(value == 12);

    // section 3, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 0, 2000);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    TEST_ASSERT(value == 2000);

    // section 3, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 1, 1000);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    TEST_ASSERT(value == 1000);

    // section 4, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 0, 3300);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    TEST_ASSERT(value == 3300);

    // section 4, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 1, 32000);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    TEST_ASSERT(value == 32000);

    // section 5, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 0, 14);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 0, value);
    TEST_ASSERT(value == 14);

    // section 5, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 1, 10);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 1, value);
    TEST_ASSERT(value == 10);
}

TEST_CASE(ErrorCheck)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    bool returnValue;

    // read
    // try calling read with invalid parameter index
    int32_t value;

    returnValue = db.read(TEST_BLOCK_INDEX, 0, sectionParams[0], value);
    TEST_ASSERT(returnValue == false);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, sectionParams[1], value);
    TEST_ASSERT(returnValue == false);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, sectionParams[2], value);
    TEST_ASSERT(returnValue == false);

    // try calling read with invalid section
    returnValue = db.read(TEST_BLOCK_INDEX, NUMBER_OF_SECTIONS, 0, value);
    TEST_ASSERT(returnValue == false);

    // try calling read with invalid block
    returnValue = db.read(NUMBER_OF_BLOCKS, 0, 0, value);
    TEST_ASSERT(returnValue == false);

    // update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, sectionParams[0], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, sectionParams[1], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, sectionParams[2], 1);
    TEST_ASSERT(returnValue == false);

    // try to init database with too many parameters
    LESSDB::section_t outOfBoundsSection[1] = { { .numberOfParameters = LESSDB_SIZE + 1,
                                                  .parameterType =
                                                      LESSDB::sectionParameterType_t::byte,
                                                  .preserveOnPartialReset = false,
                                                  .defaultValue = 1,
                                                  .autoIncrement = false,
                                                  .address = 0 } };

    LESSDB::block_t outOfBoundsLayout[1] = {
        // block 0
        {
            .address = 0,
            .numberOfSections = 1,
            .section = outOfBoundsSection,
        },
    };

    returnValue = db.setLayout(outOfBoundsLayout, 1);
    TEST_ASSERT(returnValue == false);

    // try to init database with null pointer
    returnValue = db.setLayout(NULL, 1);
    TEST_ASSERT(returnValue == false);

    // try to init database with zero blocks
    returnValue = db.setLayout(dbLayout, 0);
    TEST_ASSERT(returnValue == false);
}

TEST_CASE(ClearDB)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);
    db.clear();

    bool    returnValue;
    int32_t value;

    // verify that any read value equals 0
    // bit section
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    // byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    // half-byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    // word section
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    // dword section
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);

    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    TEST_ASSERT(value == 0);
    TEST_ASSERT(returnValue == true);
}

TEST_CASE(DBsize)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize = db.currentDBusage();

    for (int i = 0; i < NUMBER_OF_SECTIONS; i++)
    {
        switch (sectionTypes[i])
        {
        case LESSDB::sectionParameterType_t::bit:
            expectedSize += ((sectionParams[i] % 8 != 0) + sectionParams[i] / 8);
            break;

        case LESSDB::sectionParameterType_t::byte:
            expectedSize += sectionParams[i];
            break;

        case LESSDB::sectionParameterType_t::halfByte:
            expectedSize += ((sectionParams[i] % 2 != 0) + sectionParams[i] / 2);
            break;

        case LESSDB::sectionParameterType_t::word:
            expectedSize += (sectionParams[i] * 2);
            break;

        case LESSDB::sectionParameterType_t::dword:
            expectedSize += (sectionParams[i] * 4);
            break;
        }
    }

    // test uses blocks with same sections
    TEST_ASSERT(dbSize == expectedSize * NUMBER_OF_BLOCKS);
}

TEST_CASE(FactoryReset)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // block 0, section 1 is configured to preserve values after partial reset
    // write some values first
    bool    returnValue;
    int32_t value;

    returnValue = db.update(0, 1, 0, 16);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(0, 1, 0, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 16);

    returnValue = db.update(0, 1, 1, 75);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(0, 1, 1, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 75);

    returnValue = db.update(0, 1, 2, 100);
    TEST_ASSERT(returnValue == true);
    returnValue = db.read(0, 1, 2, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 100);

    // now perform partial reset
    db.initData(LESSDB::factoryResetType_t::partial);

    // verify that updated values are unchanged
    returnValue = db.read(0, 1, 0, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 16);

    returnValue = db.read(0, 1, 1, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 75);

    returnValue = db.read(0, 1, 2, value);
    TEST_ASSERT(returnValue == true);
    TEST_ASSERT(value == 100);
}

TEST_CASE(AutoIncrement)
{
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // block 0, section 1 has autoincrement configured
    // verify

    bool    returnValue;
    int32_t value;
    int32_t testValue;

    for (int i = 0; i < sectionParams[1]; i++)
    {
        testValue = i + defaultValues[1];
        returnValue = db.read(0, 1, i, value);
        TEST_ASSERT(returnValue == true);
        TEST_ASSERT(value == testValue);
    }
}

TEST_CASE(FailedRead)
{
    // configure memory read callback to always return false
    LESSDB db(memoryReadFail, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    bool    returnValue;
    int32_t value;

    // check if reading now returns an error for all sections
    // block 0
    for (int i = 0; i < NUMBER_OF_SECTIONS; i++)
    {
        returnValue = db.read(0, i, 0, value);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(FailedWrite)
{
    // configure memory write callback to always return false
    LESSDB db(memoryReadFail, memoryWriteFail, LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, NUMBER_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    bool returnValue;

    // check if writing now returns an error for all sections
    // block 0
    for (int i = 0; i < NUMBER_OF_SECTIONS; i++)
    {
        returnValue = db.update(0, i, 0, 0);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(CachingHalfByte)
{
    //half-byte and bit parameters use caching
    //once half-byte value or bit value is read, both the address and values are stored internally
    //this is used to avoid accessing the database if not needed
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);

#define TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS 4
#define TEST_CACHING_HALFBYTE_AMOUNT_OF_SECTIONS 3
#define TEST_CACHING_HALFBYTE_AMOUNT_OF_BLOCKS 1
#define TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE 10
#define TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE 4
#define TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE 0

    LESSDB::section_t cachingTestBlock0Sections[TEST_CACHING_HALFBYTE_AMOUNT_OF_SECTIONS] = {
        { .numberOfParameters = TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::halfByte,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::halfByte,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::halfByte,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },
    };

    LESSDB::block_t cachingLayout[1] = {
        // block 0
        {
            .address = 0,
            .numberOfSections = TEST_CACHING_HALFBYTE_AMOUNT_OF_SECTIONS,
            .section = cachingTestBlock0Sections,
        }
    };

    TEST_ASSERT(db.setLayout(cachingLayout, TEST_CACHING_HALFBYTE_AMOUNT_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    //the simulated database is now initialized
    //create new database object which won't initialize/write data (only the layout will be set)
    //this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db2.setLayout(cachingLayout, TEST_CACHING_HALFBYTE_AMOUNT_OF_BLOCKS) == true);

    //read the values back
    //this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db2.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
    }

    //try reading the same value twice
    LESSDB db3(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db3.setLayout(cachingLayout, TEST_CACHING_HALFBYTE_AMOUNT_OF_BLOCKS) == true);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
}

TEST_CASE(CachingBit)
{
    //half-byte and bit parameters use caching
    //once half-byte value or bit value is read, both the address and values are stored internally
    //this is used to avoid accessing the database if not needed
    LESSDB db(memoryRead, memoryWrite, LESSDB_SIZE);

#define TEST_CACHING_BIT_AMOUNT_OF_PARAMS 4
#define TEST_CACHING_BIT_AMOUNT_OF_SECTIONS 3
#define TEST_CACHING_BIT_AMOUNT_OF_BLOCKS 1
#define TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE 0
#define TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE 1
#define TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE 1

    LESSDB::section_t cachingTestBlock0Sections[TEST_CACHING_BIT_AMOUNT_OF_SECTIONS] = {
        { .numberOfParameters = TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::bit,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::bit,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },

        { .numberOfParameters = TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
          .parameterType = LESSDB::sectionParameterType_t::bit,
          .preserveOnPartialReset = false,
          .defaultValue = TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE,
          .autoIncrement = false,
          .address = 0 },
    };

    LESSDB::block_t cachingLayout[1] = {
        // block 0
        {
            .address = 0,
            .numberOfSections = TEST_CACHING_BIT_AMOUNT_OF_SECTIONS,
            .section = cachingTestBlock0Sections,
        }
    };

    TEST_ASSERT(db.setLayout(cachingLayout, TEST_CACHING_BIT_AMOUNT_OF_BLOCKS) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    //the simulated database is now initialized
    //create new database object which won't initialize/write data (only the layout will be set)
    //this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db2.setLayout(cachingLayout, TEST_CACHING_BIT_AMOUNT_OF_BLOCKS) == true);

    //read the values back
    //this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_BIT_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db2.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE);
    }

    //try reading the same value twice
    LESSDB db3(memoryRead, memoryWrite, LESSDB_SIZE);
    TEST_ASSERT(db3.setLayout(cachingLayout, TEST_CACHING_BIT_AMOUNT_OF_BLOCKS) == true);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);
}