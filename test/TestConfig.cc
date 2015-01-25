#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/DBConfig.h"
#include "../include/MockClasses.h"

using ::testing::_;
using ::testing::Return;

/**
* DBConfig should only call RawFile::Open on file.
*/
TEST(DBConfig, Read) {
    MockRawFile file;
    //EXPECT_CALL(file, Truncate());
    //EXPECT_CALL(file, Open(_))
            //.WillOnce(Return(true));
    DBConfig config;
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

