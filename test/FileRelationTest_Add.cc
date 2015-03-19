#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/FileRelationTest.h"

/**
 * Add(*Record) should add the Record to the relation
 */
TEST_F(FileRelationTest, Add1) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(relation, Add(Ref(rec)));

	EXPECT_EQ(true, rel.Add(&rec));
}

