#include <gtest/gtest.h>
#include "../src/DBMS.h"

#define NUMBER_OF_BLOCKS                    1
#define NUMBER_OF_SECTIONS                  6
#define TEST_BLOCK_INDEX                    0

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

        dbSection_t section;

        {
            section.numberOfParameters = sectionParams[0];
            section.parameterType = sectionTypes[0];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[0];
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[1];
            section.parameterType = sectionTypes[1];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[1];
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[2];
            section.parameterType = sectionTypes[2];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[2];
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[3];
            section.parameterType = sectionTypes[3];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[3];
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[4];
            section.parameterType = sectionTypes[4];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[4];
            db.addSection(0, section);

            section.numberOfParameters = sectionParams[5];
            section.parameterType = sectionTypes[5];
            section.preserveOnPartialReset = false;
            section.defaultValue = defaultValues[5];
            db.addSection(0, section);
        }

        db.commitLayout();
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
    EXPECT_EQ(value, defaultValues[1]);
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

    returnValue = db.update(TEST_BLOCK_INDEX, 0, 0, 1);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    EXPECT_EQ(returnValue, true);
    EXPECT_EQ(value, 1);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, 0, 240);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    EXPECT_EQ(value, 240);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, 0, 12);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    EXPECT_EQ(value, 12);

    returnValue = db.update(TEST_BLOCK_INDEX, 3, 0, 1000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    EXPECT_EQ(value, 1000);

    returnValue = db.update(TEST_BLOCK_INDEX, 4, 0, 32000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, 32000);

    returnValue = db.update(TEST_BLOCK_INDEX, 4, 0, 16000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, 16000);

    returnValue = db.update(TEST_BLOCK_INDEX, 4, 1, 16000);
    EXPECT_EQ(returnValue, true);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    EXPECT_EQ(value, 16000);
}

TEST_F(DBMStest, OutOfBounds)
{
    int32_t value;
    bool returnValue;

    //read
    returnValue = db.read(TEST_BLOCK_INDEX, 0, sectionParams[0], value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, sectionParams[1], value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, sectionParams[2], value);
    EXPECT_EQ(returnValue, false);

    //update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, sectionParams[0], 1);
    EXPECT_EQ(returnValue, false);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, sectionParams[1], 1);
    EXPECT_EQ(returnValue, false);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, sectionParams[2], 1);
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

    EXPECT_EQ(dbSize, expectedSize);
}

TEST_F(DBMStest, FactoryReset)
{

}