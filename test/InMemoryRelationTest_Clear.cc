#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/InMemoryRelationTest.h"

/**
 * Clear should delete all the records in relation and set them to null. It should also set the
 * index and count to null.
 */
TEST_F(InMemoryRelationTest, Clear1) {
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
	SetSize(4);

	rel.Clear();

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, GetRelation()[0] == NULL);
	EXPECT_EQ(true, GetRelation()[1] == NULL);
	EXPECT_EQ(true, GetRelation()[2] == NULL);
	EXPECT_EQ(true, GetRelation()[3] == NULL);
}

/**
 * Clear should not fail if relation is empty
 */
TEST_F(InMemoryRelationTest, Clear2) {
	StrictMock<MockRecord> rec;
	Record *test;

	SetIndex(0);
	SetSize(0);

	rel.Clear();

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(0, GetRelation().size());
}


/**
 * Clear should remove all Records from relation, even if the count is less than relation.size
 */
TEST_F(InMemoryRelationTest, Clear3) {
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

	rel.Clear();

	EXPECT_EQ(0, GetIndex());
	EXPECT_EQ(0, GetSize());
	EXPECT_EQ(0, GetMemUsed());
	EXPECT_EQ(4, GetRelation().size());
	EXPECT_EQ(true, GetRelation()[0] == NULL);
	EXPECT_EQ(true, GetRelation()[1] == NULL);
	EXPECT_EQ(true, GetRelation()[2] == NULL);
	EXPECT_EQ(true, GetRelation()[3] == NULL);
}
