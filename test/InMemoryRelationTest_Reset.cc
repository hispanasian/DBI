#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/InMemoryRelationTest.h"

/**
 * Reset should simply reset the index to 0
 */
TEST_F(InMemoryRelationTest, Reset1) {
	StrictMock<MockRecord> rec;
	Record *test;

	Record *a = new Record();
	Record *b = new Record();
	Record *c = new Record();
	Record *d = new Record();

	GetRelation().push_back(a);
	GetRelation().push_back(b);
	GetRelation().push_back(c);
	GetRelation().push_back(d);

	SetIndex(2);
	SetCount(3);

	rel.Reset();

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(3, GetCount());
}

/**
 * Reset should not fail if index is already at 0
 */
TEST_F(InMemoryRelationTest, Reset2) {
	StrictMock<MockRecord> rec;
	Record *test;

	Record *a = new Record();
	Record *b = new Record();
	Record *c = new Record();
	Record *d = new Record();

	GetRelation().push_back(a);
	GetRelation().push_back(b);
	GetRelation().push_back(c);
	GetRelation().push_back(d);

	SetIndex(0);
	SetCount(3);

	rel.Reset();

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(3, GetCount());
}
