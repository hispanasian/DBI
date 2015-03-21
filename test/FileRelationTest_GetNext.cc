#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/FileRelationTest.h"

/**
 * GetNext should call (and return the result of) relations GetNext
 */
TEST_F(FileRelationTest, GetNext1) {
	Record *rec = NULL;
	EXPECT_CALL(relation, GetNext(_)).
			WillOnce(Return(1));
	EXPECT_EQ(true, rel.GetNext(rec));
}

/**
 * GetNext should call (and return the result of) relations GetNext
 */
TEST_F(FileRelationTest, GetNext2) {
	Record *rec = NULL;
	EXPECT_CALL(relation, GetNext(_)).
			WillOnce(Return(0));
	EXPECT_EQ(false, rel.GetNext(rec));
}
