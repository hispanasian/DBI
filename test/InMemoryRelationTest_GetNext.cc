#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/InMemoryRelationTest.h"

/**
 * GetNext should return the first Record and return true when the index is 0;
 */
TEST_F(InMemoryRelationTest, GetNext1) {
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
	SetSize(2);


	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, rel.GetNext(test));
	EXPECT_EQ(1, GetIndex());
	EXPECT_EQ(2, GetSize());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(a, test);
}

/**
 * If InMemoryRelationTest has reached the count, GetNext should return false.
 */
TEST_F(InMemoryRelationTest, GetNext2) {
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

	SetIndex(3);
	SetSize(3);

	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(false, rel.GetNext(test));
	EXPECT_EQ(3, GetSize());
	EXPECT_EQ(4, GetRelation().size());
}

/**
 * If InMemoryRelationTest is empty, GetNext should return false.
 */
TEST_F(InMemoryRelationTest, GetNext3) {
	StrictMock<MockRecord> rec;
	Record *test;

	SetIndex(0);
	SetSize(0);

	EXPECT_EQ(0, GetRelation().size());
	EXPECT_EQ(false, rel.GetNext(test));
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetRelation().size());
}

/**
 * GetNext should return the next Record and return true when the index is 0;
 */
TEST_F(InMemoryRelationTest, GetNext4) {
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

	SetIndex(1);
	SetSize(4);

	EXPECT_EQ(1, GetIndex());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, rel.GetNext(test));
	EXPECT_EQ(2, GetIndex());
	EXPECT_EQ(4, GetSize());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(b, test);

	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, rel.GetNext(test));
	EXPECT_EQ(3, GetIndex());
	EXPECT_EQ(4, GetSize());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(c, test);

	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, rel.GetNext(test));
	EXPECT_EQ(4, GetIndex());
	EXPECT_EQ(4, GetSize());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(d, test);

	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(false, rel.GetNext(test));
	EXPECT_EQ(4, GetSize());
	EXPECT_EQ(4, GetRelation().size());
}
