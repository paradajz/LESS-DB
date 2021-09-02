#include "unity/Framework.h"
#include "LESSDB.h"
#include <string.h>
#include <inttypes.h>
#include <vector>

#define TEST_BLOCK_INDEX 0
#define LESSDB_SIZE      1021

namespace
{
    uint8_t memoryArray[LESSDB_SIZE];

    class DBLayoutTest
    {
        public:
        DBLayoutTest() = default;

        std::vector<size_t> sectionParams = { 5, 10, 15, 10, 15, 10 };

        std::vector<LESSDB::sectionParameterType_t> sectionTypes = {
            LESSDB::sectionParameterType_t::bit,
            LESSDB::sectionParameterType_t::byte,
            LESSDB::sectionParameterType_t::halfByte,
            LESSDB::sectionParameterType_t::word,
            LESSDB::sectionParameterType_t::dword,
            LESSDB::sectionParameterType_t::byte
        };

        std::vector<uint16_t> defaultValues = {
            1,
            10,
            15,
            20,
            25,
            30,
        };

        std::vector<LESSDB::Section> block0sections = {
            {
                sectionParams[0],
                sectionTypes[0],
                false,
                defaultValues[0],
                false,
            },

            {
                sectionParams[1],
                sectionTypes[1],
                true,
                defaultValues[1],
                true,
            },

            {
                sectionParams[2],
                sectionTypes[2],
                true,
                defaultValues[2],
                false,
            },

            {
                sectionParams[3],
                sectionTypes[3],
                true,
                defaultValues[3],
                false,
            },

            {
                sectionParams[4],
                sectionTypes[4],
                true,
                defaultValues[4],
                false,
            },

            {
                sectionParams[5],
                sectionTypes[5],
                true,
                defaultValues[5],
                false,

            }
        };

        std::vector<LESSDB::Section> block1sections = {
            {
                sectionParams[1],
                sectionTypes[1],
                false,
                defaultValues[1],
                false,

            },

            {
                sectionParams[2],
                sectionTypes[2],
                true,
                defaultValues[2],
                false,

            },

            {
                sectionParams[3],
                sectionTypes[3],
                true,
                defaultValues[3],
                false,

            },

            {
                sectionParams[4],
                sectionTypes[4],
                true,
                defaultValues[4],
                false,

            },

            {
                sectionParams[5],
                sectionTypes[5],
                true,
                defaultValues[5],
                false,

            },

            {
                sectionParams[0],
                sectionTypes[0],
                true,
                defaultValues[0],
                false,

            }
        };

        std::vector<LESSDB::Section> block2sections = {
            {
                sectionParams[2],
                sectionTypes[2],
                false,
                defaultValues[2],
                false,

            },

            {
                sectionParams[3],
                sectionTypes[3],
                true,
                defaultValues[3],
                false,

            },

            {
                sectionParams[4],
                sectionTypes[4],
                true,
                defaultValues[4],
                false,

            },

            {
                sectionParams[5],
                sectionTypes[5],
                true,
                defaultValues[5],
                false,

            },

            {
                sectionParams[0],
                sectionTypes[0],
                true,
                defaultValues[0],
                false,

            },

            {
                sectionParams[1],
                sectionTypes[1],
                true,
                defaultValues[1],
                false,

            }
        };

        std::vector<LESSDB::Section> block3sections = {
            {
                sectionParams[3],
                sectionTypes[3],
                false,
                defaultValues[3],
                false,

            },

            {
                sectionParams[4],
                sectionTypes[4],
                true,
                defaultValues[4],
                false,

            },

            {
                sectionParams[5],
                sectionTypes[5],
                true,
                defaultValues[5],
                false,

            },

            {
                sectionParams[0],
                sectionTypes[0],
                true,
                defaultValues[0],
                false,

            },

            {
                sectionParams[1],
                sectionTypes[1],
                true,
                defaultValues[1],
                false,

            },

            {
                sectionParams[2],
                sectionTypes[2],
                true,
                defaultValues[2],
                false,

            }
        };

        std::vector<LESSDB::Section> block4sections = {
            {
                sectionParams[4],
                sectionTypes[4],
                false,
                defaultValues[4],
                false,

            },

            {
                sectionParams[5],
                sectionTypes[5],
                true,
                defaultValues[5],
                false,

            },

            {
                sectionParams[0],
                sectionTypes[0],
                true,
                defaultValues[0],
                false,

            },

            {
                sectionParams[1],
                sectionTypes[1],
                true,
                defaultValues[1],
                false,

            },

            {
                sectionParams[2],
                sectionTypes[2],
                true,
                defaultValues[2],
                false,

            },

            {
                sectionParams[3],
                sectionTypes[3],
                true,
                defaultValues[3],
                false,

            }
        };

        std::vector<LESSDB::Section> block5sections = {
            {
                sectionParams[5],
                sectionTypes[5],
                false,
                defaultValues[5],
                false,

            },

            {
                sectionParams[0],
                sectionTypes[0],
                true,
                defaultValues[0],
                false,

            },

            {
                sectionParams[1],
                sectionTypes[1],
                true,
                defaultValues[1],
                false,

            },

            {
                sectionParams[2],
                sectionTypes[2],
                true,
                defaultValues[2],
                false,

            },

            {
                sectionParams[3],
                sectionTypes[3],
                true,
                defaultValues[3],
                false,

            },

            {
                sectionParams[4],
                sectionTypes[4],
                true,
                defaultValues[4],
                false,

            }
        };

        std::vector<LESSDB::Block> dbLayout = {
            // block 0
            {
                block0sections,
            },

            // block 1
            {
                block1sections,
            },

            // block 2
            {
                block2sections,
            },

            // block 3
            {
                block3sections,
            },

            // block 4
            {
                block4sections,
            },

            // block 5
            {
                block5sections,
            }
        };
    };

    bool memoryReadFail(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type)
    {
        return false;
    }

    bool memoryWriteFail(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type)
    {
        return false;
    }

    bool memoryRead(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type)
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

    class DBstorageMock : public LESSDB::StorageAccess
    {
        public:
        DBstorageMock() {}

        bool init() override
        {
            return true;
        }

        uint32_t size() override
        {
            return LESSDB_SIZE;
        }

        size_t paramUsage(LESSDB::sectionParameterType_t type) override
        {
            switch (type)
            {
            case LESSDB::sectionParameterType_t::word:
                return 2;

            case LESSDB::sectionParameterType_t::dword:
                return 4;

            case LESSDB::sectionParameterType_t::bit:
            case LESSDB::sectionParameterType_t::halfByte:
            case LESSDB::sectionParameterType_t::byte:
            default:
                return 1;
            }
        }

        bool clear() override
        {
            memset(memoryArray, 0, LESSDB_SIZE);
            return true;
        }

        bool read(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) override
        {
            return readCallback(address, value, type);
        }

        bool write(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) override
        {
            return writeCallback(address, value, type);
        }

        bool (*readCallback)(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) = nullptr;
        bool (*writeCallback)(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) = nullptr;
    } dbStorageMock;

    LESSDB db(dbStorageMock);
}    // namespace

TEST_SETUP()
{
    dbStorageMock.readCallback  = memoryRead;
    dbStorageMock.writeCallback = memoryWrite;
}

TEST_CASE(Read)
{
    DBLayoutTest testLayout;
    DBLayoutTest testLayoutDiffAdd;
    int32_t      value;

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // bit section
    for (int i = 0; i < testLayout.sectionParams[0]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 0, i, value) == true);
        TEST_ASSERT(value == testLayout.defaultValues[0]);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < testLayout.sectionParams[1]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 1, i, value) == true);
        TEST_ASSERT(value == testLayout.defaultValues[1] + i);
    }

    // half-byte section
    for (int i = 0; i < testLayout.sectionParams[2]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 2, i, value) == true);
        TEST_ASSERT(value == testLayout.defaultValues[2]);
    }

    // word section
    for (int i = 0; i < testLayout.sectionParams[3]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 3, i, value) == true);
        TEST_ASSERT(value == testLayout.defaultValues[3]);
    }

    // dword section
    for (int i = 0; i < testLayout.sectionParams[4]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 4, i, value) == true);
        TEST_ASSERT(value == testLayout.defaultValues[4]);
    }

    // try reading directly
    value = db.read(TEST_BLOCK_INDEX, 1, 0);
    TEST_ASSERT(value == testLayout.defaultValues[1]);

    // perform the same round of tests with different starting point
    TEST_ASSERT(db.setLayout(testLayoutDiffAdd.dbLayout, 100) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // bit section
    for (int i = 0; i < testLayoutDiffAdd.sectionParams[0]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 0, i, value) == true);
        TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[0]);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < testLayoutDiffAdd.sectionParams[1]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 1, i, value) == true);
        TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[1] + i);
    }

    // half-byte section
    for (int i = 0; i < testLayoutDiffAdd.sectionParams[2]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 2, i, value) == true);
        TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[2]);
    }

    // word section
    for (int i = 0; i < testLayoutDiffAdd.sectionParams[3]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 3, i, value) == true);
        TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[3]);
    }

    // dword section
    for (int i = 0; i < testLayoutDiffAdd.sectionParams[4]; i++)
    {
        TEST_ASSERT(db.read(TEST_BLOCK_INDEX, 4, i, value) == true);
        TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[4]);
    }

    // try reading directly
    value = db.read(TEST_BLOCK_INDEX, 1, 0);
    TEST_ASSERT(value == testLayoutDiffAdd.defaultValues[1]);
}

TEST_CASE(Update)
{
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

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
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;
    size_t       invalidBlock   = testLayout.dbLayout.size();
    size_t       invalidSection = testLayout.dbLayout.at(0).section().size();

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // read
    // try calling read with invalid parameter index

    returnValue = db.read(TEST_BLOCK_INDEX, 0, testLayout.sectionParams[0], value);
    TEST_ASSERT(returnValue == false);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, testLayout.sectionParams[1], value);
    TEST_ASSERT(returnValue == false);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, testLayout.sectionParams[2], value);
    TEST_ASSERT(returnValue == false);

    // try calling read with invalid section
    returnValue = db.read(0, invalidSection, 0, value);
    TEST_ASSERT(returnValue == false);

    // try calling read with invalid block
    returnValue = db.read(invalidBlock, 0, 0, value);
    TEST_ASSERT(returnValue == false);

    // update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, testLayout.sectionParams[0], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, testLayout.sectionParams[1], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, testLayout.sectionParams[2], 1);
    TEST_ASSERT(returnValue == false);

    // try to init database with too many parameters
    std::vector<LESSDB::Section> outOfBoundsSection = {
        {
            LESSDB_SIZE + 1,
            LESSDB::sectionParameterType_t::byte,
            false,
            1,
            false,
        }
    };

    std::vector<LESSDB::Block> outOfBoundsLayout = {
        // block 0
        {
            outOfBoundsSection,
        },
    };

    returnValue = db.setLayout(outOfBoundsLayout, 0);
    TEST_ASSERT(returnValue == false);

    // try to init database with zero blocks
    std::vector<LESSDB::Block> zeroBlocks;

    returnValue = db.setLayout(zeroBlocks, 0);
    TEST_ASSERT(returnValue == false);
}

TEST_CASE(ClearDB)
{
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    db.clear();

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
    DBLayoutTest testLayout;
    size_t       totalBlocks   = testLayout.dbLayout.size();
    size_t       totalSections = testLayout.dbLayout.at(0).section().size();

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize       = db.currentDBsize();

    for (int i = 0; i < totalSections; i++)
    {
        switch (testLayout.sectionTypes[i])
        {
        case LESSDB::sectionParameterType_t::bit:
            expectedSize += ((testLayout.sectionParams[i] % 8 != 0) + testLayout.sectionParams[i] / 8);
            break;

        case LESSDB::sectionParameterType_t::byte:
            expectedSize += testLayout.sectionParams[i];
            break;

        case LESSDB::sectionParameterType_t::halfByte:
            expectedSize += ((testLayout.sectionParams[i] % 2 != 0) + testLayout.sectionParams[i] / 2);
            break;

        case LESSDB::sectionParameterType_t::word:
            expectedSize += (testLayout.sectionParams[i] * 2);
            break;

        case LESSDB::sectionParameterType_t::dword:
            expectedSize += (testLayout.sectionParams[i] * 4);
            break;
        }
    }

    // test uses blocks with same sections
    TEST_ASSERT(dbSize == expectedSize * totalBlocks);
}

TEST_CASE(FactoryReset)
{
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // block 0, section 1 is configured to preserve values after partial reset
    // write some values first

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
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;
    int32_t      testValue;

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // block 0, section 1 has autoincrement configured
    // verify

    for (int i = 0; i < testLayout.sectionParams[1]; i++)
    {
        testValue   = i + testLayout.defaultValues[1];
        returnValue = db.read(0, 1, i, value);
        TEST_ASSERT(returnValue == true);
        TEST_ASSERT(value == testValue);
    }
}

TEST_CASE(FailedRead)
{
    DBLayoutTest testLayout;
    int32_t      value;
    bool         returnValue;
    size_t       totalSections = testLayout.dbLayout.at(0).section().size();

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // configure memory read callback to always return false
    dbStorageMock.readCallback = memoryReadFail;

    // check if reading now returns an error for all sections
    // block 0
    for (int i = 0; i < totalSections; i++)
    {
        returnValue = db.read(0, i, 0, value);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(FailedWrite)
{
    DBLayoutTest testLayout;
    bool         returnValue;
    size_t       totalSections = testLayout.dbLayout.at(0).section().size();

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(testLayout.dbLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // configure memory write callback to always return false
    dbStorageMock.writeCallback = memoryWriteFail;

    // check if writing now returns an error for all sections
    // block 0
    for (int i = 0; i < totalSections; i++)
    {
        returnValue = db.update(0, i, 0, 0);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(CachingHalfByte)
{
    int32_t readValue;

    //half-byte and bit parameters use caching
    //once half-byte value or bit value is read, both the address and values are stored internally
    //this is used to avoid accessing the database if not needed

#define TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS        4
#define TEST_CACHING_HALFBYTE_AMOUNT_OF_BLOCKS        1
#define TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE 10
#define TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE 4
#define TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE 0

    std::vector<LESSDB::Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            false,
            TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE,
            false,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            false,
            TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE,
            false,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            false,
            TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE,
            false,
        },
    };

    std::vector<LESSDB::Block> cachingLayout = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    std::vector<LESSDB::Block> cachingLayout2 = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    std::vector<LESSDB::Block> cachingLayout3 = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    TEST_ASSERT(db.setLayout(cachingLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    //the simulated database is now initialized
    //create new database object which won't initialize/write data (only the layout will be set)
    //this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    TEST_ASSERT(db2.setLayout(cachingLayout2, 0) == true);

    //read the values back
    //this will verify that the values are read properly and that caching doesn't influence the readout

    for (int i = 0; i < TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db2.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
    }

    //try reading the same value twice
    LESSDB db3(dbStorageMock);
    TEST_ASSERT(db3.setLayout(cachingLayout3, 0) == true);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
}

TEST_CASE(CachingBit)
{
    int32_t readValue;

    //half-byte and bit parameters use caching
    //once half-byte value or bit value is read, both the address and values are stored internally
    //this is used to avoid accessing the database if not needed

#define TEST_CACHING_BIT_AMOUNT_OF_PARAMS        4
#define TEST_CACHING_BIT_AMOUNT_OF_BLOCKS        1
#define TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE 0
#define TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE 1
#define TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE 1

    std::vector<LESSDB::Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            false,
            TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE,
            false,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            false,
            TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE,
            false,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            false,
            TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE,
            false,
        },
    };

    std::vector<LESSDB::Block> cachingLayout = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    std::vector<LESSDB::Block> cachingLayout2 = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    std::vector<LESSDB::Block> cachingLayout3 = {
        // block 0
        {
            cachingTestBlock0Sections,
        }
    };

    TEST_ASSERT(db.setLayout(cachingLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    //the simulated database is now initialized
    //create new database object which won't initialize/write data (only the layout will be set)
    //this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    TEST_ASSERT(db.setLayout(cachingLayout2, 0) == true);

    //read the values back
    //this will verify that the values are read properly and that caching doesn't influence the readout

    for (int i = 0; i < TEST_CACHING_BIT_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE);
    }

    //try reading the same value twice
    LESSDB db3(dbStorageMock);
    TEST_ASSERT(db3.setLayout(cachingLayout3, 0) == true);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);
}