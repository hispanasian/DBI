#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/InMemoryRelationTest.h"

/**
 * Add should return true and add the Record if it does not exceed the memSize
 */
TEST_F(InMemoryRelationTest, Add1) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(rec, Size()).
			WillRepeatedly(Return(50));

	EXPECT_EQ(0, GetRelation().size());
	EXPECT_EQ(true, rel.Add(&rec));
	EXPECT_EQ(1, GetRelation().size());
}

/**
 * Add should return false and not add the Record if it exceeds the memSize
 */
TEST_F(InMemoryRelationTest, Add2) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(rec, Size()).
			WillRepeatedly(Return(150));

	EXPECT_EQ(0, GetRelation().size());
	EXPECT_EQ(false, rel.Add(&rec));
	EXPECT_EQ(0, GetRelation().size());
}
