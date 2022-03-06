#include "unity/Framework.h"
#include "LESSDB.h"
#include <string.h>

#define TEST_BLOCK_INDEX 0
#define LESSDB_SIZE      1021

namespace
{
    uint8_t memoryArray[LESSDB_SIZE];

    std::vector<size_t> sectionParams = { 5, 10, 15, 10, 15, 10 };

    const std::vector<LESSDB::sectionParameterType_t> sectionTypes = {
        LESSDB::sectionParameterType_t::bit,
        LESSDB::sectionParameterType_t::byte,
        LESSDB::sectionParameterType_t::halfByte,
        LESSDB::sectionParameterType_t::word,
        LESSDB::sectionParameterType_t::dword,
        LESSDB::sectionParameterType_t::byte
    };

    const std::vector<uint16_t> defaultValues = {
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
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },

        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::enable,
            defaultValues[1],
        },

        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },

        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },

        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },

        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },
    };

    std::vector<LESSDB::Section> block1sections = {
        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[1],
        },

        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },

        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },

        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },

        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },

        {
            sectionParams[0],
            sectionTypes[0],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },
    };

    std::vector<LESSDB::Section> block2sections = {
        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },

        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },

        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },

        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },

        {
            sectionParams[0],
            sectionTypes[0],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },

        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[1],
        },
    };

    std::vector<LESSDB::Section> block3sections = {
        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },

        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },

        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },

        {
            sectionParams[0],
            sectionTypes[0],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },

        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[1],
        },

        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },
    };

    std::vector<LESSDB::Section> block4sections = {
        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },

        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },

        {
            sectionParams[0],
            sectionTypes[0],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },

        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[1],
        },

        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },

        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },
    };

    std::vector<LESSDB::Section> block5sections = {
        {
            sectionParams[5],
            sectionTypes[5],
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[5],
        },

        {
            sectionParams[0],
            sectionTypes[0],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[0],
        },

        {
            sectionParams[1],
            sectionTypes[1],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[1],
        },

        {
            sectionParams[2],
            sectionTypes[2],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[2],
        },

        {
            sectionParams[3],
            sectionTypes[3],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[3],
        },

        {
            sectionParams[4],
            sectionTypes[4],
            LESSDB::preserveSetting_t::enable,
            LESSDB::autoIncrementSetting_t::disable,
            defaultValues[4],
        },
    };

    std::vector<LESSDB::Block> dbLayout = {
        {
            block0sections,
        },

        {
            block1sections,
        },

        {
            block2sections,
        },

        {
            block3sections,
        },

        {
            block4sections,
        },

        {
            block5sections,
        },
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

    TEST_ASSERT(db.dbSize() == LESSDB_SIZE);
    TEST_ASSERT(db.setLayout(dbLayout, 0) == true);

    db.initData(LESSDB::factoryResetType_t::full);
}

TEST_CASE(Read)
{
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
    TEST_ASSERT(db.setLayout(dbLayout, 100) == true);
    db.initData(LESSDB::factoryResetType_t::full);

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
}

TEST_CASE(Update)
{
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
    returnValue = db.read(TEST_BLOCK_INDEX, dbLayout.size(), 0, value);
    TEST_ASSERT(returnValue == false);

    // try calling read with invalid block
    returnValue = db.read(dbLayout.size(), 0, 0, value);
    TEST_ASSERT(returnValue == false);

    // update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, sectionParams[0], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, sectionParams[1], 1);
    TEST_ASSERT(returnValue == false);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, sectionParams[2], 1);
    TEST_ASSERT(returnValue == false);

    // try to init database with too many parameters
    std::vector<LESSDB::Section> outOfBoundsSection = {
        {
            LESSDB_SIZE + 1,
            LESSDB::sectionParameterType_t::byte,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            1,
        },
    };

    std::vector<LESSDB::Block> outOfBoundsLayout = {
        {
            outOfBoundsSection,
        },
    };

    returnValue = db.setLayout(outOfBoundsLayout, 0);
    TEST_ASSERT(returnValue == false);

    // try to init database with zero blocks
    std::vector<LESSDB::Block> emptyLayout = {};
    returnValue                            = db.setLayout(emptyLayout, 0);
    TEST_ASSERT(returnValue == false);
}

TEST_CASE(ClearDB)
{
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
    // test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize       = db.currentDBsize();

    for (int i = 0; i < dbLayout.size(); i++)
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
    TEST_ASSERT(dbSize == expectedSize * dbLayout.size());
}

TEST_CASE(FactoryReset)
{
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
    // block 0, section 1 has autoincrement configured
    // verify

    bool    returnValue;
    int32_t value;
    int32_t testValue;

    for (int i = 0; i < sectionParams[1]; i++)
    {
        testValue   = i + defaultValues[1];
        returnValue = db.read(0, 1, i, value);
        TEST_ASSERT(returnValue == true);
        TEST_ASSERT(value == testValue);
    }
}

TEST_CASE(FailedRead)
{
    // configure memory read callback to always return false
    dbStorageMock.readCallback = memoryReadFail;

    bool    returnValue;
    int32_t value;

    // check if reading now returns an error for all sections
    // block 0
    for (int i = 0; i < dbLayout.size(); i++)
    {
        returnValue = db.read(0, i, 0, value);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(FailedWrite)
{
    // configure memory write callback to always return false
    dbStorageMock.writeCallback = memoryWriteFail;

    bool returnValue;

    // check if writing now returns an error for all sections
    // block 0
    for (int i = 0; i < dbLayout.size(); i++)
    {
        returnValue = db.update(0, i, 0, 0);
        TEST_ASSERT(returnValue == false);
    }
}

TEST_CASE(CachingHalfByte)
{
    // half-byte and bit parameters use caching
    // once half-byte value or bit value is read, both the address and values are stored internally
    // this is used to avoid accessing the database if not needed

    static constexpr size_t TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS        = 4;
    static constexpr size_t TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE = 10;
    static constexpr size_t TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE = 4;
    static constexpr size_t TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE = 0;

    std::vector<LESSDB::Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::halfByte,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE,
        },
    };

    std::vector<LESSDB::Block> cachingLayout = {
        {
            cachingTestBlock0Sections,
        },
    };

    TEST_ASSERT(db.setLayout(cachingLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    TEST_ASSERT(db2.setLayout(cachingLayout, 0) == true);

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db2.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
    }

    // try reading the same value twice
    LESSDB db3(dbStorageMock);
    TEST_ASSERT(db3.setLayout(cachingLayout, 0) == true);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE);
}

TEST_CASE(CachingBit)
{
    // half-byte and bit parameters use caching
    // once half-byte value or bit value is read, both the address and values are stored internally
    // this is used to avoid accessing the database if not needed

    static constexpr size_t TEST_CACHING_BIT_AMOUNT_OF_PARAMS        = 4;
    static constexpr size_t TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE = 0;
    static constexpr size_t TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE = 1;
    static constexpr size_t TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE = 1;

    std::vector<LESSDB::Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            LESSDB::sectionParameterType_t::bit,
            LESSDB::preserveSetting_t::disable,
            LESSDB::autoIncrementSetting_t::disable,
            TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE,
        },
    };

    std::vector<LESSDB::Block> cachingLayout = {
        {
            cachingTestBlock0Sections,
        },
    };

    TEST_ASSERT(db.setLayout(cachingLayout, 0) == true);
    db.initData(LESSDB::factoryResetType_t::full);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    TEST_ASSERT(db.setLayout(cachingLayout, 0) == true);

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_BIT_AMOUNT_OF_PARAMS; i++)
    {
        TEST_ASSERT(db.read(0, 0, i, readValue) == true);
        TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE);
    }

    // try reading the same value twice
    LESSDB db3(dbStorageMock);
    TEST_ASSERT(db3.setLayout(cachingLayout, 0) == true);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);

    TEST_ASSERT(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue) == true);
    TEST_ASSERT(readValue == TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE);
}