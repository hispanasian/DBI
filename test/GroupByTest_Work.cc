#include "GroupByTest.h"

// No records
TEST_F(GroupByTest, Work1) {
	EXPECT_CALL(in, Remove(&rec)).
			WillOnce(Return(0));

	op.Work(in, out, groupAtts, func, rec, prev, comp);
	EXPECT_EQ(0, out.Remove(&rec));
}

// 1 records
TEST_F(GroupByTest, Work2) {

}

// All records in same group
TEST_F(GroupByTest, Work3) {

}

// All records in differnet group
TEST_F(GroupByTest, Work4) {

}

// 4 different groups
TEST_F(GroupByTest, Work5) {

}