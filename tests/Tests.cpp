#include <gtest/gtest.h>
#include "../src/DBMS.h"

#define NUMBER_OF_BLOCKS                    1

#define SECTION_0_PARAMETERS                10
#define SECTION_1_PARAMETERS                6
#define SECTION_2_PARAMETERS                33

DBMS db;

class DBMStest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
        db.addBlocks(NUMBER_OF_BLOCKS);

        dbSection_t section;

        {
            section.numberOfParameters = SECTION_0_PARAMETERS;
            section.parameterType = DWORD_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 10;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_1_PARAMETERS;
            section.parameterType = DWORD_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 15;
            db.addSection(0, section);

            section.numberOfParameters = SECTION_2_PARAMETERS;
            section.parameterType = DWORD_PARAMETER;
            section.preserveOnPartialReset = false;
            section.defaultValue = 20;
            db.addSection(0, section);
        }

        db.commitLayout();
        db.initData();
    }

    virtual void TearDown()
    {
        
    }
};

TEST_F(DBMStest, Init)
{
    int32_t value = db.read(0, 2, 0);

    EXPECT_EQ(value, 20);
}
