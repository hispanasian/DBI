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
* DBConfig should only call RawFile::Close on file when there have been no changes.
*/
TEST(DBConfig, Close1) {
    MockRawFile file;
}

/**
* DBConfig should first write changes made to file when it has made a change, after which it should
* close file.
*/
TEST(DBConfig, Close2) {

}

