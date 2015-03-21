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

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(0, GetRelation().size());
	EXPECT_EQ(true, rel.Add(&rec));
	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(1, GetSize());
	EXPECT_EQ(50, GetMemUsed());
	EXPECT_EQ(1, GetRelation().size());
}

/**
 * Add should return false and not add the Record if it exceeds the memSize
 */
TEST_F(InMemoryRelationTest, Add2) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(rec, Size()).
			WillRepeatedly(Return(150));

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(0, GetRelation().size());
	EXPECT_EQ(false, rel.Add(&rec));
	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(0, GetRelation().size());
}


/**
 * Add should return true and add the Record if it does not exceed the memSize. Furthermore, it
 * should remove any element at the current position according to the index.
 */
TEST_F(InMemoryRelationTest, Add3) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(rec, Size()).
			WillRepeatedly(Return(50));

	GetRelation().push_back(NULL);
	GetRelation().push_back(NULL);
	GetRelation().push_back(NULL);

	SetSize(1);
	SetIndex(0);
	SetMemUsed(10);

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(1, GetSize());
	EXPECT_EQ(10, GetMemUsed());
	EXPECT_EQ(3, GetRelation().size());
	EXPECT_EQ(true, rel.Add(&rec));
	EXPECT_NE(false, GetRelation()[1] != NULL);
	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(2, GetSize());
	EXPECT_EQ(60, GetMemUsed());
	EXPECT_EQ(3, GetRelation().size());
}
