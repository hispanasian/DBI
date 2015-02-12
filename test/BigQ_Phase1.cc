#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/BigQ.h"


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::Test;
using ::testing::StrictMock;

/**
* Fixture for DBConfig that provides access to map.
*/
class Phase1Test: public ::testing::Test {
public:

	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
    OrderMaker order;
    StrictMock<MockComparisonEngine> comp;
    StrictMock<File> file;
    vector<int> runPages;
    StrictMock<Page> page;
};

TEST_F(Phase1Test, Test1) {

	EXPECT_EQ(5, Phase1(in, out, order, comp, file, runPages, page, 2));
}
