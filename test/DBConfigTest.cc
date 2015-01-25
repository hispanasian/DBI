#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/DBConfig.h"
#include "../include/MockClasses.h"
#include <map>

using ::testing::_;
using ::testing::Return;

/**
* Fixture for DBConfig that provides access to map.
*/
class DBConfigTest: public ::testing::Test {
public:
    DBConfig config;
    map<std::string,std::string> getMap();
protected:
    //virtual void SetUp() {}
    //virtual void TearDown() {}
};

/**
* Returns the map of config
*/
map<std::string,std::string> DBConfigTest::getMap() {
    return config.map;
}

/**
* DBConfig::Read should take an open RawFile that has no contents and
*/
TEST_F(DBConfigTest, Read1) {
    MockRawFile file;

    EXPECT_EQ(true, config.Read(file));
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

