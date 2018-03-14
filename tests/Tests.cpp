#include <gtest/gtest.h>
#include "../src/DBMS.h"

#define NUMBER_OF_BLOCKS                    1

#define TEST_BLOCK_INDEX                    0

#define SECTION_0_PARAMETERS                5
#define SECTION_1_PARAMETERS                10
#define SECTION_2_PARAMETERS                15
#define SECTION_3_PARAMETERS                20
#define SECTION_4_PARAMETERS                25

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
            section.numberOfParameters = SECTION_0_PARAMETERS;
            section.parameterType = BIT_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 1;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_1_PARAMETERS;
            section.parameterType = BYTE_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 10;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_2_PARAMETERS;
            section.parameterType = HALFBYTE_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 15;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_3_PARAMETERS;
            section.parameterType = WORD_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 20;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_4_PARAMETERS;
            section.parameterType = DWORD_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 25;
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

    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    EXPECT_EQ(value, 10);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    EXPECT_EQ(value, 15);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    EXPECT_EQ(value, 20);
    EXPECT_EQ(returnValue, true);

    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    EXPECT_EQ(value, 25);
    EXPECT_EQ(returnValue, true);
}

TEST_F(DBMStest, OutOfBounds)
{
    int32_t value;
    bool returnValue;

    returnValue = db.read(TEST_BLOCK_INDEX, 0, SECTION_0_PARAMETERS, value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, SECTION_1_PARAMETERS, value);
    EXPECT_EQ(returnValue, false);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, SECTION_2_PARAMETERS, value);
    EXPECT_EQ(returnValue, false);
}