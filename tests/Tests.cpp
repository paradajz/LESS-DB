#include <gtest/gtest.h>
#include "../src/DBMS.h"

#define NUMBER_OF_BLOCKS                    5
#define NUMBER_OF_SECTIONS                  6
#define TEST_BLOCK_INDEX                    0

uint8_t memoryArray[EEPROM_SIZE];

bool memoryReadFail(uint32_t address, sectionParameterType_t type, int32_t &value)
{
    return false;
}

bool memoryWriteFail(uint32_t address, int32_t value, sectionParameterType_t type)
{
    return false;
}

bool memoryRead(uint32_t address, sectionParameterType_t type, int32_t &value)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        value = memoryArray[address];
        break;

        case WORD_PARAMETER:
        value = memoryArray[address+1];
        value <<= 8;
        value |= memoryArray[address+0];
        break;

        default:
        // case DWORD_PARAMETER:
        value = memoryArray[address+3];
        value <<= 8;
        value |= memoryArray[address+2];
        value <<= 8;
        value |= memoryArray[address+1];
        value <<= 8;
        value |= memoryArray[address+0];
        break;
    }

    return true;
}

bool memoryWrite(uint32_t address, int32_t value, sectionParameterType_t type)
{
    switch(type)
    {
        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        case HALFBYTE_PARAMETER:
        memoryArray[address] = value;
        break;

        case WORD_PARAMETER:
        memoryArray[address+0] = (value >> 0) & (uint16_t)0xFF;
        memoryArray[address+1] = (value >> 8) & (uint16_t)0xFF;
        break;

        case DWORD_PARAMETER:
        memoryArray[address+0] = (value >> 0) & (uint32_t)0xFF;
        memoryArray[address+1] = (value >> 8) & (uint32_t)0xFF;
        memoryArray[address+2] = (value >> 16) & (uint32_t)0xFF;
        memoryArray[address+3] = (value >> 24) & (uint32_t)0xFF;
        break;
    }

    return true;
}

const uint16_t sectionParams[NUMBER_OF_SECTIONS] =
{
    5,
    10,
    15,
    20,
    25,
    30
};

const sectionParameterType_t sectionTypes[NUMBER_OF_SECTIONS] =
{
    BIT_PARAMETER,
    BYTE_PARAMETER,
    HALFBYTE_PARAMETER,
    WORD_PARAMETER,
    DWORD_PARAMETER,
    BYTE_PARAMETER
};

const uint16_t defaultValues[NUMBER_OF_SECTIONS] =
{
    1,
    10,
    15,
    20,
    25,
    30
};

DBMS db;

class DBMStest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
        db.init();
        db.addBlocks(NUMBER_OF_BLOCKS);
        db.setHandleRead(memoryRead);
        db.setHandleWrite(memoryWrite);

        dbSection_t section;

        //block 0
        {
            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            section.autoIncrement = false;
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = true;
            section.defaultValue = defaultValues[1];
            section.autoIncrement = true;
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[2];
            section.autoIncrement = false;
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[3];
            section.autoIncrement = false;
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[4];
            section.autoIncrement = false;
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[5];
            section.autoIncrement = false;
            db.addSection(0, section);
        }

        //block 1
        {
            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[1];
            section.autoIncrement = false;
            db.addSection(1, section);

            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = true;
            section.defaultValue = defaultValues[2];
            section.autoIncrement = true;
            db.addSection(1, section);

            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[3];
            section.autoIncrement = false;
            db.addSection(1, section);

            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[4];
            section.autoIncrement = false;
            db.addSection(1, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[5];
            section.autoIncrement = false;
            db.addSection(1, section);

            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            section.autoIncrement = false;
            db.addSection(1, section);
        }

        //block 2
        {
            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[2];
            section.autoIncrement = false;
            db.addSection(2, section);

            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = true;
            section.defaultValue = defaultValues[3];
            section.autoIncrement = true;
            db.addSection(2, section);

            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[4];
            section.autoIncrement = false;
            db.addSection(2, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[5];
            section.autoIncrement = false;
            db.addSection(2, section);

            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            section.autoIncrement = false;
            db.addSection(2, section);

            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[1];
            section.autoIncrement = false;
            db.addSection(2, section);
        }

        //block 3
        {
            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[3];
            section.autoIncrement = false;
            db.addSection(3, section);

            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = true;
            section.defaultValue = defaultValues[4];
            section.autoIncrement = true;
            db.addSection(3, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[5];
            section.autoIncrement = false;
            db.addSection(3, section);

            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            section.autoIncrement = false;
            db.addSection(3, section);

            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[1];
            section.autoIncrement = false;
            db.addSection(3, section);

            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[2];
            section.autoIncrement = false;
            db.addSection(3, section);
        }

        //block 4
        {
            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[4];
            section.autoIncrement = false;
            db.addSection(4, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = true;
            section.defaultValue = defaultValues[5];
            section.autoIncrement = true;
            db.addSection(4, section);

            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            section.autoIncrement = false;
            db.addSection(4, section);

            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[1];
            section.autoIncrement = false;
            db.addSection(4, section);

            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[2];
            section.autoIncrement = false;
            db.addSection(4, section);

            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[3];
            section.autoIncrement = false;
            db.addSection(4, section);
        }

        db.initData();
    }

    virtual void TearDown()
    {
        
    }
};

TEST_F(DBMStest, Read)
{
    int32_t value;
    bool returnValue;

    //bit section
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    EXPECT_EQ(value, defaultValues[0]);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    EXPECT_EQ(value, defaultValues[0]);
    EXPECT_EQ(returnValue, true);

    //byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    EXPECT_EQ(value, defaultValues[1]);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    //autoincrement is enabled for this section
    EXPECT_EQ(value, defaultValues[1]+1);
    EXPECT_EQ(returnValue, true);

    //half-byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    EXPECT_EQ(value, defaultValues[2]);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    EXPECT_EQ(value, defaultValues[2]);
    EXPECT_EQ(returnValue, true);

    //word section
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    EXPECT_EQ(value, defaultValues[3]);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    EXPECT_EQ(value, defaultValues[3]);
    EXPECT_EQ(returnValue, true);

    //dword section
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, defaultValues[4]);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    EXPECT_EQ(value, defaultValues[4]);
    EXPECT_EQ(returnValue, true);
}

TEST_F(DBMStest, Update)
{
    int32_t value;
    bool returnValue;

    //section 0, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 0, 1);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 1);

    //section 0, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 1, 0);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 0);

    //section 1, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 0, 240);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    EXPECT_EQ(value, 240);

    //section 1, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 1, 143);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    EXPECT_EQ(value, 143);

    //section 2, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 0, 4);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    EXPECT_EQ(value, 4);

    //section 2, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 1, 12);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    EXPECT_EQ(value, 12);

    //section 3, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 0, 2000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    EXPECT_EQ(value, 2000);

    //section 3, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 1, 1000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    EXPECT_EQ(value, 1000);

    //section 4, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 0, 3300);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, 3300);

    //section 4, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 1, 32000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    EXPECT_EQ(value, 32000);

    //section 5, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 0, 14);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 0, value);
    EXPECT_EQ(value, 14);

    //section 5, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 1, 10);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 1, value);
    EXPECT_EQ(value, 10);
}

TEST_F(DBMStest, OutOfBounds)
{
    int32_t value;
    bool returnValue;

    //read
    //try calling read with invalid parameter index
    returnValue = db.read(TEST_BLOCK_INDEX, 0, sectionParams[0], value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, sectionParams[1], value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, sectionParams[2], value);
    EXPECT_EQ(returnValue, false);

    //try calling read with invalid section
    returnValue = db.read(TEST_BLOCK_INDEX, NUMBER_OF_SECTIONS, 0, value);
    EXPECT_EQ(returnValue, false);

    //try calling read with invalid block
    returnValue = db.read(NUMBER_OF_BLOCKS, 0, 0, value);
    EXPECT_EQ(returnValue, false);

    //update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, sectionParams[0], 1);
    EXPECT_EQ(returnValue, false);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, sectionParams[1], 1);
    EXPECT_EQ(returnValue, false);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, sectionParams[2], 1);
    EXPECT_EQ(returnValue, false);

    //try adding another block
    returnValue = db.addBlocks(1);
    EXPECT_EQ(returnValue, true);

    //add another section configured to cause eeprom size overload
    dbSection_t section;
    section.numberOfParameters = (EEPROM_SIZE - db.getDBsize()) + 1;
    section.parameterType = BYTE_PARAMETER;
    returnValue = db.addSection(NUMBER_OF_BLOCKS, section);
    EXPECT_EQ(returnValue, false);
}

TEST_F(DBMStest, ClearDB)
{
    bool returnValue;
    int32_t value;

    db.clear();

    //verify that any read value equals 0
    //bit section
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    //byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    //half-byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    //word section
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    //dword section
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    EXPECT_EQ(value, 0);
    EXPECT_EQ(returnValue, true);
}

TEST_F(DBMStest, DBsize)
{
    //test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize = db.getDBsize();

    for (int i=0; i<NUMBER_OF_SECTIONS; i++)
    {
        switch(sectionTypes[i])
        {
            case BIT_PARAMETER:
            expectedSize += ((sectionParams[i] % 8 != 0) + sectionParams[i]/8);
            break;

            case BYTE_PARAMETER:
            expectedSize += sectionParams[i];
            break;

            case HALFBYTE_PARAMETER:
            expectedSize += (sectionParams[i] / 2);
            break;

            case WORD_PARAMETER:
            expectedSize += (sectionParams[i] * 2);
            break;

            case DWORD_PARAMETER:
            expectedSize += (sectionParams[i] * 4);
            break;
        }
    }

    //test uses two blocks with same sections
    EXPECT_EQ(dbSize, expectedSize*NUMBER_OF_BLOCKS);
}

TEST_F(DBMStest, FactoryReset)
{
    //block 0, section 1 is configured to preserve values after partial reset
    //write some values first
    bool returnValue;
    int32_t value;

    returnValue = db.update(0, 1, 0, 16);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(0, 1, 0, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 16);

    returnValue = db.update(0, 1, 1, 75);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(0, 1, 1, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 75);

    returnValue = db.update(0, 1, 2, 100);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(0, 1, 2, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 100);

    //now perform partial reset
    db.initData(initPartial);

    //verify that updated values are unchanged
    returnValue = db.read(0, 1, 0, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 16);

    returnValue = db.read(0, 1, 1, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 75);

    returnValue = db.read(0, 1, 2, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 100);
}

TEST_F(DBMStest, AutoIncrement)
{
    //block 0, section 1 has autoincrement configure
    //verify

    bool returnValue;
    int32_t value;
    int32_t testValue;

    for (int i=0; i<sectionParams[1]; i++)
    {
        testValue = i+defaultValues[1];
        returnValue = db.read(0, 1, i, value);
        EXPECT_EQ(returnValue, true);
        EXPECT_EQ(value, testValue);
    }
}

TEST_F(DBMStest, TooManyBlocks)
{
    db.init();
    bool returnValue = db.addBlocks(DBMS_MAX_BLOCKS+1);

    EXPECT_EQ(returnValue, false);
}

TEST_F(DBMStest, TooManySections)
{
    bool returnValue;

    db.init();
    returnValue = db.addBlocks(1);
    EXPECT_EQ(returnValue, true);

    dbSection_t section;

    section.numberOfParameters = 1;
    section.parameterType = BYTE_PARAMETER;

    for (int i=0; i<=DBMS_MAX_SECTIONS; i++)
    {
        returnValue = db.addSection(0, section);
        EXPECT_EQ(returnValue, true);
    }

    returnValue = db.addSection(0, section);
    EXPECT_EQ(returnValue, false);
}

TEST_F(DBMStest, FailedRead)
{
    bool returnValue;
    int32_t value;

    //configure memory read callback to always return false
    db.setHandleRead(memoryReadFail);

    //check if reading now returns an error for all sections
    //block 0
    for (int i=0; i<NUMBER_OF_SECTIONS; i++)
    {
        returnValue = db.read(0, i, 0, value);
        EXPECT_EQ(returnValue, false);
    }
}

TEST_F(DBMStest, FailedWrite)
{
    bool returnValue;

    //configure memory write callback to always return false
    db.setHandleWrite(memoryWriteFail);

    //check if writing now returns an error for all sections
    //block 0
    for (int i=0; i<NUMBER_OF_SECTIONS; i++)
    {
        returnValue = db.update(0, i, 0, 0);
        EXPECT_EQ(returnValue, false);
    }
}