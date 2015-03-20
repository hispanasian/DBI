#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/JoinRelationTest.h"

/**
 * Add should, if it does not surpass the memory limit, not change the delegate to a FileRelation
 * and it should return true.
 */
TEST_F(JoinRelationTest, Add1) {
	StrictMock<MockRecord> rec;
	StrictMock<MockRelation> del;

	SetDelegate(&del);

	EXPECT_CALL(del, Add(&rec)).
			WillOnce(Return(true));

	EXPECT_EQ(true, rel.Add(&rec));
	EXPECT_EQ(true, rel.IsMemoryMode());

	SetDelegate(NULL);
}

/**
 * Add should return false if memory is false.
 */
TEST_F(JoinRelationTest, Add2) {
	StrictMock<MockRecord> rec;
	StrictMock<MockRelation> del;

	SetDelegate(&del);
	SetMemory(false);

	EXPECT_CALL(del, Add(&rec)).
			WillOnce(Return(true));

	EXPECT_EQ(false, rel.Add(&rec));
	EXPECT_EQ(false, rel.IsMemoryMode());

	SetDelegate(NULL);
}

/**
 * Add should, if it surpasses the memory limit, change the delegate to a FileRelation and populate
 * it.
 */
TEST_F(JoinRelationTest, Add3) {
	Record *rec = new Record(5);

	MockRelation *del = new MockRelation();

	SetDelegate(del);

	EXPECT_CALL((*del), Add(rec)).
			WillOnce(Return(false));

	EXPECT_CALL(*del, GetNext(NotNull())).
			WillOnce(Return(false));

	EXPECT_CALL(*del, Reset());

	EXPECT_EQ(false, rel.Add(rec));
	EXPECT_EQ(false, rel.IsMemoryMode());
}
