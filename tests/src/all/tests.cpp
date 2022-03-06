#include "framework/Framework.h"
#include "LESSDB.h"
#include <string.h>

namespace
{
    class DatabaseTest : public ::testing::Test
    {
        public:
        static constexpr uint32_t LESSDB_SIZE = 1021;

        protected:
        void SetUp() override
        {
            ASSERT_EQ(db.dbSize(), LESSDB_SIZE);
            ASSERT_TRUE(db.setLayout(dbLayout));
            ASSERT_TRUE(db.initData(LESSDB::factoryResetType_t::full));
        }

        void TearDown() override
        {
        }

        class DBstorageMock : public LESSDB::StorageAccess
        {
            public:
            DBstorageMock()
            {
                _readCallback = [this](uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) {
                    return memoryRead(address, value, type);
                };

                _writeCallback = [this](uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) {
                    return memoryWrite(address, value, type);
                };
            }

            bool init() override
            {
                return true;
            }

            uint32_t size() override
            {
                return DatabaseTest::LESSDB_SIZE;
            }

            bool clear() override
            {
                memset(_memoryArray, 0, DatabaseTest::LESSDB_SIZE);
                return true;
            }

            bool read(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) override
            {
                return _readCallback(address, value, type);
            }

            bool write(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) override
            {
                return _writeCallback(address, value, type);
            }

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
                {
                    value = _memoryArray[address];
                }
                break;

                case LESSDB::sectionParameterType_t::word:
                {
                    value = _memoryArray[address + 1];
                    value <<= 8;
                    value |= _memoryArray[address + 0];
                }
                break;

                default:
                {
                    // case LESSDB::sectionParameterType_t::dword:
                    value = _memoryArray[address + 3];
                    value <<= 8;
                    value |= _memoryArray[address + 2];
                    value <<= 8;
                    value |= _memoryArray[address + 1];
                    value <<= 8;
                    value |= _memoryArray[address + 0];
                }
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
                {
                    _memoryArray[address] = value;
                }
                break;

                case LESSDB::sectionParameterType_t::word:
                {
                    _memoryArray[address + 0] = (value >> 0) & (uint16_t)0xFF;
                    _memoryArray[address + 1] = (value >> 8) & (uint16_t)0xFF;
                }
                break;

                case LESSDB::sectionParameterType_t::dword:
                {
                    _memoryArray[address + 0] = (value >> 0) & (uint32_t)0xFF;
                    _memoryArray[address + 1] = (value >> 8) & (uint32_t)0xFF;
                    _memoryArray[address + 2] = (value >> 16) & (uint32_t)0xFF;
                    _memoryArray[address + 3] = (value >> 24) & (uint32_t)0xFF;
                }
                break;
                }

                return true;
            }

            std::function<bool(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type)> _readCallback;
            std::function<bool(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type)>  _writeCallback;

            private:
            uint8_t _memoryArray[DatabaseTest::LESSDB_SIZE];
        };

        static constexpr size_t TEST_BLOCK_INDEX = 0;
        std::vector<size_t>     sectionParams    = {
            5,
            10,
            15,
            10,
            15,
            10,
        };

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

        DBstorageMock dbStorageMock;
        LESSDB        db = LESSDB(dbStorageMock);
    };
}    // namespace

TEST_F(DatabaseTest, Read)
{
    int32_t value;

    // bit section
    for (int i = 0; i < sectionParams[0]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 0, i, value));
        ASSERT_EQ(defaultValues[0], value);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < sectionParams[1]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 1, i, value));
        ASSERT_EQ(defaultValues[1] + i, value);
    }

    // half-byte section
    for (int i = 0; i < sectionParams[2]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 2, i, value));
        ASSERT_EQ(defaultValues[2], value);
    }

    // word section
    for (int i = 0; i < sectionParams[3]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 3, i, value));
        ASSERT_EQ(defaultValues[3], value);
    }

    // dword section
    for (int i = 0; i < sectionParams[4]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 4, i, value));
        ASSERT_EQ(defaultValues[4], value);
    }

    // try reading directly
    value = db.read(TEST_BLOCK_INDEX, 1, 0);
    ASSERT_EQ(defaultValues[1], value);

    // perform the same round of tests with different starting point
    ASSERT_TRUE(db.setLayout(dbLayout, 100));
    db.initData(LESSDB::factoryResetType_t::full);

    // bit section
    for (int i = 0; i < sectionParams[0]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 0, i, value));
        ASSERT_EQ(defaultValues[0], value);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < sectionParams[1]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 1, i, value));
        ASSERT_EQ(defaultValues[1] + i, value);
    }

    // half-byte section
    for (int i = 0; i < sectionParams[2]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 2, i, value));
        ASSERT_EQ(defaultValues[2], value);
    }

    // word section
    for (int i = 0; i < sectionParams[3]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 3, i, value));
        ASSERT_EQ(defaultValues[3], value);
    }

    // dword section
    for (int i = 0; i < sectionParams[4]; i++)
    {
        ASSERT_TRUE(db.read(TEST_BLOCK_INDEX, 4, i, value));
        ASSERT_EQ(defaultValues[4], value);
    }

    // try reading directly
    value = db.read(TEST_BLOCK_INDEX, 1, 0);
    ASSERT_EQ(defaultValues[1], value);
}

TEST_F(DatabaseTest, Update)
{
    int32_t value;
    bool    returnValue;

    // section 0, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 0, 1);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(1, value);

    // section 0, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 0, 1, 0);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(0, value);

    // section 1, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 0, 240);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    ASSERT_EQ(240, value);

    // section 1, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 1, 1, 143);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    ASSERT_EQ(143, value);

    // section 2, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 0, 4);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    ASSERT_EQ(4, value);

    // section 2, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 2, 1, 12);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    ASSERT_EQ(12, value);

    // section 3, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 0, 2000);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    ASSERT_EQ(2000, value);

    // section 3, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 3, 1, 1000);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    ASSERT_EQ(1000, value);

    // section 4, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 0, 3300);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    ASSERT_EQ(3300, value);

    // section 4, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 4, 1, 32000);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    ASSERT_EQ(32000, value);

    // section 5, index 0
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 0, 14);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 0, value);
    ASSERT_EQ(14, value);

    // section 5, index 1
    returnValue = db.update(TEST_BLOCK_INDEX, 5, 1, 10);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(TEST_BLOCK_INDEX, 5, 1, value);
    ASSERT_EQ(10, value);
}

TEST_F(DatabaseTest, ErrorCheck)
{
    bool returnValue;

    // read
    // try calling read with invalid parameter index
    int32_t value;

    returnValue = db.read(TEST_BLOCK_INDEX, 0, sectionParams[0], value);
    ASSERT_FALSE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, sectionParams[1], value);
    ASSERT_FALSE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, sectionParams[2], value);
    ASSERT_FALSE(returnValue);

    // try calling read with invalid section
    returnValue = db.read(TEST_BLOCK_INDEX, dbLayout.size(), 0, value);
    ASSERT_FALSE(returnValue);

    // try calling read with invalid block
    returnValue = db.read(dbLayout.size(), 0, 0, value);
    ASSERT_FALSE(returnValue);

    // update
    returnValue = db.update(TEST_BLOCK_INDEX, 0, sectionParams[0], 1);
    ASSERT_FALSE(returnValue);

    returnValue = db.update(TEST_BLOCK_INDEX, 1, sectionParams[1], 1);
    ASSERT_FALSE(returnValue);

    returnValue = db.update(TEST_BLOCK_INDEX, 2, sectionParams[2], 1);
    ASSERT_FALSE(returnValue);

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
    ASSERT_FALSE(returnValue);

    // try to init database with zero blocks
    std::vector<LESSDB::Block> emptyLayout = {};
    returnValue                            = db.setLayout(emptyLayout, 0);
    ASSERT_FALSE(returnValue);
}

TEST_F(DatabaseTest, ClearDB)
{
    db.clear();

    bool    returnValue;
    int32_t value;

    // verify that any read value equals 0
    // bit section
    returnValue = db.read(TEST_BLOCK_INDEX, 0, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 0, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 1, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 1, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // half-byte section
    returnValue = db.read(TEST_BLOCK_INDEX, 2, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 2, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // word section
    returnValue = db.read(TEST_BLOCK_INDEX, 3, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 3, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // dword section
    returnValue = db.read(TEST_BLOCK_INDEX, 4, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = db.read(TEST_BLOCK_INDEX, 4, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);
}

TEST_F(DatabaseTest, DBsize)
{
    // test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize       = db.currentDBsize();

    for (int i = 0; i < dbLayout.size(); i++)
    {
        switch (sectionTypes[i])
        {
        case LESSDB::sectionParameterType_t::bit:
        {
            expectedSize += ((sectionParams[i] % 8 != 0) + sectionParams[i] / 8);
        }
        break;

        case LESSDB::sectionParameterType_t::byte:
        {
            expectedSize += sectionParams[i];
        }
        break;

        case LESSDB::sectionParameterType_t::halfByte:
        {
            expectedSize += ((sectionParams[i] % 2 != 0) + sectionParams[i] / 2);
        }
        break;

        case LESSDB::sectionParameterType_t::word:
        {
            expectedSize += (sectionParams[i] * 2);
        }
        break;

        case LESSDB::sectionParameterType_t::dword:
        {
            expectedSize += (sectionParams[i] * 4);
        }
        break;
        }
    }

    // test uses blocks with same sections
    ASSERT_EQ(dbSize, expectedSize * dbLayout.size());
}

TEST_F(DatabaseTest, FactoryReset)
{
    // block 0, section 1 is configured to preserve values after partial reset
    // write some values first
    bool    returnValue;
    int32_t value;

    returnValue = db.update(0, 1, 0, 16);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(0, 1, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(16, value);

    returnValue = db.update(0, 1, 1, 75);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(0, 1, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(75, value);

    returnValue = db.update(0, 1, 2, 100);
    ASSERT_TRUE(returnValue);
    returnValue = db.read(0, 1, 2, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(100, value);

    // now perform partial reset
    db.initData(LESSDB::factoryResetType_t::partial);

    // verify that updated values are unchanged
    returnValue = db.read(0, 1, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(16, value);

    returnValue = db.read(0, 1, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(75, value);

    returnValue = db.read(0, 1, 2, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(100, value);
}

TEST_F(DatabaseTest, AutoIncrement)
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
        ASSERT_TRUE(returnValue);
        ASSERT_EQ(testValue, value);
    }
}

TEST_F(DatabaseTest, FailedRead)
{
    // configure memory read callback to always return false
    dbStorageMock._readCallback = [this](uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) {
        return dbStorageMock.memoryReadFail(address, value, type);
    };

    bool    returnValue;
    int32_t value;

    // check if reading now returns an error for all sections
    // block 0
    for (int i = 0; i < dbLayout.size(); i++)
    {
        returnValue = db.read(0, i, 0, value);
        ASSERT_FALSE(returnValue);
    }
}

TEST_F(DatabaseTest, FailedWrite)
{
    // configure memory write callback to always return false
    dbStorageMock._writeCallback = [this](uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) {
        return dbStorageMock.memoryWriteFail(address, value, type);
    };

    bool returnValue;

    // check if writing now returns an error for all sections
    // block 0
    for (int i = 0; i < dbLayout.size(); i++)
    {
        returnValue = db.update(0, i, 0, 0);
        ASSERT_FALSE(returnValue);
    }
}

TEST_F(DatabaseTest, CachingHalfByte)
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

    ASSERT_TRUE(db.setLayout(cachingLayout, 0));
    db.initData(LESSDB::factoryResetType_t::full);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    ASSERT_TRUE(db2.setLayout(cachingLayout, 0));

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS; i++)
    {
        ASSERT_TRUE(db2.read(0, 0, i, readValue));
        ASSERT_EQ(TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE, readValue);
    }

    // try reading the same value twice
    LESSDB db3(dbStorageMock);
    ASSERT_TRUE(db3.setLayout(cachingLayout, 0));

    ASSERT_TRUE(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE, readValue);

    ASSERT_TRUE(db3.read(0, 0, TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE, readValue);
}

TEST_F(DatabaseTest, CachingBit)
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

    ASSERT_TRUE(db.setLayout(cachingLayout, 0));
    db.initData(LESSDB::factoryResetType_t::full);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LESSDB db2(dbStorageMock);
    ASSERT_TRUE(db.setLayout(cachingLayout, 0));

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    int32_t readValue;

    for (int i = 0; i < TEST_CACHING_BIT_AMOUNT_OF_PARAMS; i++)
    {
        ASSERT_TRUE(db.read(0, 0, i, readValue));
        ASSERT_EQ(TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE, readValue);
    }

    // try reading the same value twice
    LESSDB db3(dbStorageMock);
    ASSERT_TRUE(db3.setLayout(cachingLayout, 0));

    ASSERT_TRUE(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE, readValue);

    ASSERT_TRUE(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE, readValue);
}