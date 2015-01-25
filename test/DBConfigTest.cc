#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/DBConfig.h"
#include "../include/MockClasses.h"
#include <map>

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

/**
* Fixture for DBConfig that provides access to map.
*/
class DBConfigTest: public ::testing::Test {
public:
    DBConfig config;
    map<std::string,std::string> Map();
protected:
    //virtual void SetUp() {}
    //virtual void TearDown() {}
};

/**
* Returns the map of config
*/
map<std::string,std::string> DBConfigTest::Map() {
    return config.map;
}

/**
* DBConfig::Read should take an open RawFile that has no contents (or simply an un-opened RawFile),
* read it once (via readline) and then Close the file. If Close poses no problem, then Read return
* true and DBConfig should have an empty map.
*/
TEST_F(DBConfigTest, Read1) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return(""));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(true));
    }
    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(true, Map().empty());
}

/**
* DBConfig::Read should take an open RawFile that starts at the beginning with 3 elements correctly
* formatted and add them to map. It should then add them to the map and finally close the file and
* return true.
*/
TEST_F(DBConfigTest, Read2) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("key=value"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("this=that"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("kit=kat"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return(""));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(true));
    }
    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(3, Map().size());

    std::string value("value");
    std::string that("that");
    std::string kat("kat");
    EXPECT_EQ(0, value.compare(Map()["key"]));
    EXPECT_EQ(0, that.compare(Map()["this"]));
    EXPECT_EQ(0, kat.compare(Map()["kit"]));
}

/**
* DBCofnig::Read should return false if Close returns false.
*/
TEST_F(DBConfigTest, Read3) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return(""));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(false));
    }
    EXPECT_EQ(false, config.Read(file));
}

/**
* DBConfig::Read should return false if it encounters a line that does not match the format:
* "key=value".
*/
TEST_F(DBConfigTest, Read4) {
    MockRawFile file;
    {
        InSequence seq;
        /* Some valid lines */
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("key=val"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("good=not bad")); // Spaces!
        /* Invalid line */
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("invalid"));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(true));
    }

    EXPECT_EQ(false, config.Read(file));
}

/**
* DBConfig::Read should return true if it encounters multiple keys. It should over-write the old
* key in the file.
*/
TEST_F(DBConfigTest, Read5) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("a=A"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("yes=no"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("check=good"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("check=bad"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("no=yes"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return(""));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(true));
    }

    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(4, Map().size());

    std::string A("A");
    std::string no("no");
    std::string good("good");
    std::string yes("yes");
    EXPECT_EQ(0, A.compare(Map()["a"]));
    EXPECT_EQ(0, no.compare(Map()["yes"]));
    EXPECT_EQ(0, good.compare(Map()["check"]));
    EXPECT_EQ(0, yes.compare(Map()["no"]));
}

/**
* DBConfig::Read should not fail if it finds an extra '=' character. The first '=' character will
* act as the delimiter for key,value and any remaining '=' characters will be used as part of val.
*/
TEST_F(DBConfigTest, Read6) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("non=sense"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("this=is=dumb"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return("not=dumb"));
        EXPECT_CALL(file, ReadLine()).
                WillOnce(Return(""));
        EXPECT_CALL(file, Close()).
                WillOnce(Return(true));
    }

    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(3, Map().size());

    std::string sense("sense");
    std::string isDumb("is=dumb");
    std::string dumb("dumb");
    EXPECT_EQ(0, sense.compare(Map()["non"]));
    EXPECT_EQ(0, isDumb.compare(Map()["this"]));
    EXPECT_EQ(0, dumb.compare(Map()["not"]));
}
