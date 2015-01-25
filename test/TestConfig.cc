#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Config.h"
#include "../include/MockClasses.h"

using ::testing::_;
using ::testing::Return;

/**
* Config should only call RawFile::Open on file.
*/
TEST(Config, Open) {
    MockRawFile file;
    Config config(file);
    EXPECT_CALL(file, Truncate());
    //EXPECT_CALL(file, Open(_))
            //.WillOnce(Return(true));
    //EXPECT_EQ(true, config.Open("some/path"));
}

/**
* Config should only call RawFile::Close on file when there have been no changes.
*/
TEST(Config, Close1) {
    MockRawFile file;
}

/**
* Config should first write changes made to file when it has made a change, after which it should
* close file.
*/
TEST(Config, Close2) {

}

