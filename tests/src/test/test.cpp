#include "tests/common.h"
#include "lib/lessdb/lessdb.h"

using namespace lib::lessdb;

namespace
{
    class DatabaseTest : public ::testing::Test
    {
        public:
        static constexpr uint32_t LESSDB_SIZE = 1021;

        protected:
        void SetUp() override
        {
            ASSERT_EQ(_lessdb.dbSize(), LESSDB_SIZE);
            ASSERT_TRUE(_lessdb.setLayout(DB_LAYOUT));
            ASSERT_TRUE(_lessdb.initData(factoryResetType_t::FULL));
        }

        void TearDown() override
        {}

        class HwaLessDb : public Hwa
        {
            public:
            HwaLessDb()
            {
                _readCallback = [this](uint32_t address, uint32_t& value, sectionParameterType_t type)
                {
                    return memoryRead(address, value, type);
                };

                _writeCallback = [this](uint32_t address, uint32_t value, sectionParameterType_t type)
                {
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

            bool read(uint32_t address, uint32_t& value, sectionParameterType_t type) override
            {
                return _readCallback(address, value, type);
            }

            bool write(uint32_t address, uint32_t value, sectionParameterType_t type) override
            {
                return _writeCallback(address, value, type);
            }

            bool memoryReadFail(uint32_t address, uint32_t& value, sectionParameterType_t type)
            {
                return false;
            }

            bool memoryWriteFail(uint32_t address, uint32_t value, sectionParameterType_t type)
            {
                return false;
            }

            bool memoryRead(uint32_t address, uint32_t& value, sectionParameterType_t type)
            {
                switch (type)
                {
                case sectionParameterType_t::BIT:
                case sectionParameterType_t::BYTE:
                case sectionParameterType_t::HALF_BYTE:
                {
                    value = _memoryArray[address];
                }
                break;

                case sectionParameterType_t::WORD:
                {
                    value = _memoryArray[address + 1];
                    value <<= 8;
                    value |= _memoryArray[address + 0];
                }
                break;

                default:
                {
                    // case sectionParameterType_t::DWORD:
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

            bool memoryWrite(uint32_t address, uint32_t value, sectionParameterType_t type)
            {
                switch (type)
                {
                case sectionParameterType_t::BIT:
                case sectionParameterType_t::BYTE:
                case sectionParameterType_t::HALF_BYTE:
                {
                    _memoryArray[address] = value;
                }
                break;

                case sectionParameterType_t::WORD:
                {
                    _memoryArray[address + 0] = (value >> 0) & (uint16_t)0xFF;
                    _memoryArray[address + 1] = (value >> 8) & (uint16_t)0xFF;
                }
                break;

                case sectionParameterType_t::DWORD:
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

            std::function<bool(uint32_t address, uint32_t& value, sectionParameterType_t type)> _readCallback;
            std::function<bool(uint32_t address, uint32_t value, sectionParameterType_t type)>  _writeCallback;

            private:
            uint8_t _memoryArray[DatabaseTest::LESSDB_SIZE];
        };

        static constexpr size_t TEST_BLOCK_INDEX = 0;

        const std::vector<size_t> SECTION_PARAMS = {
            5,
            10,
            15,
            10,
            15,
            10,
        };

        const std::vector<sectionParameterType_t> SECTION_TYPES = {
            sectionParameterType_t::BIT,
            sectionParameterType_t::BYTE,
            sectionParameterType_t::HALF_BYTE,
            sectionParameterType_t::WORD,
            sectionParameterType_t::DWORD,
            sectionParameterType_t::BYTE
        };

        const std::vector<uint16_t> DEFAULT_VALUES = {
            1,
            10,
            15,
            20,
            25,
            30,
        };

        std::vector<Section> BLOCK_0_SECTIONS = {
            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },

            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::ENABLE,
                DEFAULT_VALUES[1],
            },

            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },

            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },

            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },

            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },
        };

        std::vector<Section> BLOCK_1_SECTIONS = {
            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[1],
            },

            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },

            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },

            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },

            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },

            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },
        };

        std::vector<Section> BLOCK_2_SECTIONS = {
            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },

            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },

            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },

            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },

            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },

            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[1],
            },
        };

        std::vector<Section> BLOCK_3_SECTIONS = {
            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },

            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },

            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },

            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },

            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[1],
            },

            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },
        };

        std::vector<Section> BLOCK_4_SECTIONS = {
            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },

            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },

            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },

            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[1],
            },

            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },

            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },
        };

        std::vector<Section> BLOCK_5_SECTIONS = {
            {
                SECTION_PARAMS[5],
                SECTION_TYPES[5],
                preserveSetting_t::DISABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[5],
            },

            {
                SECTION_PARAMS[0],
                SECTION_TYPES[0],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[0],
            },

            {
                SECTION_PARAMS[1],
                SECTION_TYPES[1],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[1],
            },

            {
                SECTION_PARAMS[2],
                SECTION_TYPES[2],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[2],
            },

            {
                SECTION_PARAMS[3],
                SECTION_TYPES[3],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[3],
            },

            {
                SECTION_PARAMS[4],
                SECTION_TYPES[4],
                preserveSetting_t::ENABLE,
                autoIncrementSetting_t::DISABLE,
                DEFAULT_VALUES[4],
            },
        };

        std::vector<Block> DB_LAYOUT = {
            {
                BLOCK_0_SECTIONS,
            },

            {
                BLOCK_1_SECTIONS,
            },

            {
                BLOCK_2_SECTIONS,
            },

            {
                BLOCK_3_SECTIONS,
            },

            {
                BLOCK_4_SECTIONS,
            },

            {
                BLOCK_5_SECTIONS,
            },
        };

        HwaLessDb _hwa;
        LessDb    _lessdb = LessDb(_hwa);
    };
}    // namespace

TEST_F(DatabaseTest, Read)
{
    uint32_t value;

    // bit section
    for (int i = 0; i < SECTION_PARAMS[0]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 0, i, value));
        ASSERT_EQ(DEFAULT_VALUES[0], value);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < SECTION_PARAMS[1]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 1, i, value));
        ASSERT_EQ(DEFAULT_VALUES[1] + i, value);
    }

    // half-byte section
    for (int i = 0; i < SECTION_PARAMS[2]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 2, i, value));
        ASSERT_EQ(DEFAULT_VALUES[2], value);
    }

    // word section
    for (int i = 0; i < SECTION_PARAMS[3]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 3, i, value));
        ASSERT_EQ(DEFAULT_VALUES[3], value);
    }

    // ::DWORD section
    for (int i = 0; i < SECTION_PARAMS[4]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 4, i, value));
        ASSERT_EQ(DEFAULT_VALUES[4], value);
    }

    // try reading directly
    value = _lessdb.read(TEST_BLOCK_INDEX, 1, 0);
    ASSERT_EQ(DEFAULT_VALUES[1], value);

    // perform the same round of tests with different starting point
    ASSERT_TRUE(_lessdb.setLayout(DB_LAYOUT, 100));
    _lessdb.initData(factoryResetType_t::FULL);

    // bit section
    for (int i = 0; i < SECTION_PARAMS[0]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 0, i, value));
        ASSERT_EQ(DEFAULT_VALUES[0], value);
    }

    // byte section
    // autoincrement is enabled for this section
    for (int i = 0; i < SECTION_PARAMS[1]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 1, i, value));
        ASSERT_EQ(DEFAULT_VALUES[1] + i, value);
    }

    // half-byte section
    for (int i = 0; i < SECTION_PARAMS[2]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 2, i, value));
        ASSERT_EQ(DEFAULT_VALUES[2], value);
    }

    // word section
    for (int i = 0; i < SECTION_PARAMS[3]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 3, i, value));
        ASSERT_EQ(DEFAULT_VALUES[3], value);
    }

    // ::DWORD section
    for (int i = 0; i < SECTION_PARAMS[4]; i++)
    {
        ASSERT_TRUE(_lessdb.read(TEST_BLOCK_INDEX, 4, i, value));
        ASSERT_EQ(DEFAULT_VALUES[4], value);
    }

    // try reading directly
    value = _lessdb.read(TEST_BLOCK_INDEX, 1, 0);
    ASSERT_EQ(DEFAULT_VALUES[1], value);
}

TEST_F(DatabaseTest, Update)
{
    uint32_t value;
    bool     returnValue;

    // section 0, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 0, 0, 1);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 0, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(1, value);

    // section 0, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 0, 1, 0);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 0, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(0, value);

    // section 1, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 1, 0, 240);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 1, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(240, value);

    // section 1, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 1, 1, 143);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 1, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(143, value);

    // section 2, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 2, 0, 4);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 2, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(4, value);

    // section 2, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 2, 1, 12);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 2, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(12, value);

    // section 3, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 3, 0, 2000);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 3, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(2000, value);

    // section 3, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 3, 1, 1000);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 3, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(1000, value);

    // section 4, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 4, 0, 3300);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 4, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(3300, value);

    // section 4, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 4, 1, 32000);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 4, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(32000, value);

    // section 5, index 0
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 5, 0, 14);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 5, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(14, value);

    // section 5, index 1
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 5, 1, 10);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 5, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(10, value);
}

TEST_F(DatabaseTest, ErrorCheck)
{
    bool returnValue;

    // read
    // try calling read with invalid parameter index
    uint32_t value;

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 0, SECTION_PARAMS[0], value);
    ASSERT_FALSE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 1, SECTION_PARAMS[1], value);
    ASSERT_FALSE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 2, SECTION_PARAMS[2], value);
    ASSERT_FALSE(returnValue);

    // try calling read with invalid section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, DB_LAYOUT.size(), 0, value);
    ASSERT_FALSE(returnValue);

    // try calling read with invalid block
    returnValue = _lessdb.read(DB_LAYOUT.size(), 0, 0, value);
    ASSERT_FALSE(returnValue);

    // update
    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 0, SECTION_PARAMS[0], 1);
    ASSERT_FALSE(returnValue);

    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 1, SECTION_PARAMS[1], 1);
    ASSERT_FALSE(returnValue);

    returnValue = _lessdb.update(TEST_BLOCK_INDEX, 2, SECTION_PARAMS[2], 1);
    ASSERT_FALSE(returnValue);

    // try to init database with too many parameters
    std::vector<Section> outOfBoundsSection = {
        {
            LESSDB_SIZE + 1,
            sectionParameterType_t::BYTE,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            1,
        },
    };

    std::vector<Block> outOfBoundsLayout = {
        {
            outOfBoundsSection,
        },
    };

    returnValue = _lessdb.setLayout(outOfBoundsLayout, 0);
    ASSERT_FALSE(returnValue);

    // try to init database with zero blocks
    std::vector<Block> emptyLayout = {};
    returnValue                    = _lessdb.setLayout(emptyLayout, 0);
    ASSERT_FALSE(returnValue);
}

TEST_F(DatabaseTest, ClearDB)
{
    _lessdb.clear();

    bool     returnValue;
    uint32_t value;

    // verify that any read value equals 0
    // bit section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 0, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 0, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // byte section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 1, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 1, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // half-byte section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 2, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 2, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // word section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 3, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 3, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    // ::DWORD section
    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 4, 0, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);

    returnValue = _lessdb.read(TEST_BLOCK_INDEX, 4, 1, value);
    ASSERT_EQ(0, value);
    ASSERT_TRUE(returnValue);
}

TEST_F(DatabaseTest, DBsize)
{
    // test if calculated database size matches the one returned from object
    int expectedSize = 0;
    int dbSize       = _lessdb.currentDatabaseSize();

    for (int i = 0; i < DB_LAYOUT.size(); i++)
    {
        switch (SECTION_TYPES[i])
        {
        case sectionParameterType_t::BIT:
        {
            expectedSize += ((SECTION_PARAMS[i] % 8 != 0) + SECTION_PARAMS[i] / 8);
        }
        break;

        case sectionParameterType_t::BYTE:
        {
            expectedSize += SECTION_PARAMS[i];
        }
        break;

        case sectionParameterType_t::HALF_BYTE:
        {
            expectedSize += ((SECTION_PARAMS[i] % 2 != 0) + SECTION_PARAMS[i] / 2);
        }
        break;

        case sectionParameterType_t::WORD:
        {
            expectedSize += (SECTION_PARAMS[i] * 2);
        }
        break;

        case sectionParameterType_t::DWORD:
        {
            expectedSize += (SECTION_PARAMS[i] * 4);
        }
        break;
        }
    }

    // test uses blocks with same sections
    ASSERT_EQ(dbSize, expectedSize * DB_LAYOUT.size());
}

TEST_F(DatabaseTest, FactoryReset)
{
    // block 0, section 1 is configured to preserve values after partial reset
    // write some values first
    bool     returnValue;
    uint32_t value;

    returnValue = _lessdb.update(0, 1, 0, 16);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(0, 1, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(16, value);

    returnValue = _lessdb.update(0, 1, 1, 75);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(0, 1, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(75, value);

    returnValue = _lessdb.update(0, 1, 2, 100);
    ASSERT_TRUE(returnValue);
    returnValue = _lessdb.read(0, 1, 2, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(100, value);

    // now perform partial reset
    _lessdb.initData(factoryResetType_t::PARTIAL);

    // verify that updated values are unchanged
    returnValue = _lessdb.read(0, 1, 0, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(16, value);

    returnValue = _lessdb.read(0, 1, 1, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(75, value);

    returnValue = _lessdb.read(0, 1, 2, value);
    ASSERT_TRUE(returnValue);
    ASSERT_EQ(100, value);
}

TEST_F(DatabaseTest, AutoIncrement)
{
    // block 0, section 1 has autoincrement configured
    // verify

    bool     returnValue;
    uint32_t value;
    uint32_t testValue;

    for (int i = 0; i < SECTION_PARAMS[1]; i++)
    {
        testValue   = i + DEFAULT_VALUES[1];
        returnValue = _lessdb.read(0, 1, i, value);
        ASSERT_TRUE(returnValue);
        ASSERT_EQ(testValue, value);
    }
}

TEST_F(DatabaseTest, FailedRead)
{
    // configure memory read callback to always return false
    _hwa._readCallback = [this](uint32_t address, uint32_t& value, sectionParameterType_t type)
    {
        return _hwa.memoryReadFail(address, value, type);
    };

    bool     returnValue;
    uint32_t value;

    // check if reading now returns an error for all sections
    // block 0
    for (int i = 0; i < DB_LAYOUT.size(); i++)
    {
        returnValue = _lessdb.read(0, i, 0, value);
        ASSERT_FALSE(returnValue);
    }
}

TEST_F(DatabaseTest, FailedWrite)
{
    // configure memory write callback to always return false
    _hwa._writeCallback =
        [this](uint32_t address, uint32_t value, sectionParameterType_t type)
    {
        return _hwa.memoryWriteFail(address, value, type);
    };

    bool returnValue;

    // check if writing now returns an error for all sections
    // block 0
    for (int i = 0; i < DB_LAYOUT.size(); i++)
    {
        returnValue = _lessdb.update(0, i, 0, 0);
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

    std::vector<Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            sectionParameterType_t::HALF_BYTE,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            sectionParameterType_t::HALF_BYTE,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_HALFBYTE_SECTION_1_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS,
            sectionParameterType_t::HALF_BYTE,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_HALFBYTE_SECTION_2_DEFAULT_VALUE,
        },
    };

    std::vector<Block> cachingLayout = {
        {
            cachingTestBlock0Sections,
        },
    };

    ASSERT_TRUE(_lessdb.setLayout(cachingLayout, 0));
    _lessdb.initData(factoryResetType_t::FULL);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LessDb db2(_hwa);
    ASSERT_TRUE(db2.setLayout(cachingLayout, 0));

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    uint32_t readValue;

    for (int i = 0; i < TEST_CACHING_HALFBYTE_AMOUNT_OF_PARAMS; i++)
    {
        ASSERT_TRUE(db2.read(0, 0, i, readValue));
        ASSERT_EQ(TEST_CACHING_HALFBYTE_SECTION_0_DEFAULT_VALUE, readValue);
    }

    // try reading the same value twice
    LessDb db3(_hwa);
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

    std::vector<Section> cachingTestBlock0Sections = {
        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            sectionParameterType_t::BIT,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            sectionParameterType_t::BIT,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE,
        },

        {
            TEST_CACHING_BIT_AMOUNT_OF_PARAMS,
            sectionParameterType_t::BIT,
            preserveSetting_t::DISABLE,
            autoIncrementSetting_t::DISABLE,
            TEST_CACHING_BIT_SECTION_2_DEFAULT_VALUE,
        },
    };

    std::vector<Block> cachingLayout = {
        {
            cachingTestBlock0Sections,
        },
    };

    ASSERT_TRUE(_lessdb.setLayout(cachingLayout, 0));
    _lessdb.initData(factoryResetType_t::FULL);

    // the simulated database is now initialized
    // create new database object which won't initialize/write data (only the layout will be set)
    // this is used so that database doesn't call ::update function which resets the cached address
    LessDb db2(_hwa);
    ASSERT_TRUE(_lessdb.setLayout(cachingLayout, 0));

    // read the values back
    // this will verify that the values are read properly and that caching doesn't influence the readout
    uint32_t readValue;

    for (int i = 0; i < TEST_CACHING_BIT_AMOUNT_OF_PARAMS; i++)
    {
        ASSERT_TRUE(_lessdb.read(0, 0, i, readValue));
        ASSERT_EQ(TEST_CACHING_BIT_SECTION_0_DEFAULT_VALUE, readValue);
    }

    // try reading the same value twice
    LessDb db3(_hwa);
    ASSERT_TRUE(db3.setLayout(cachingLayout, 0));

    ASSERT_TRUE(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE, readValue);

    ASSERT_TRUE(db3.read(0, 1, TEST_CACHING_BIT_AMOUNT_OF_PARAMS - 1, readValue));
    ASSERT_EQ(TEST_CACHING_BIT_SECTION_1_DEFAULT_VALUE, readValue);
}