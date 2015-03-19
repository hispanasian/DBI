#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/FileRelationTest.h"

/**
 * Reset should call MoveFirst on relation.
 */
TEST_F(FileRelationTest, Reset1) {
	EXPECT_CALL(relation, MoveFirst());
	rel.Reset();
}
