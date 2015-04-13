#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * CopyRel should copy the attributes and number of tuples from "test" to "new"
 */
TEST_F(StatisticsTest, CopyRel1) {
	map["test"].numTuples = 5;
	map["test"].atts["some"] = 10;
	map["test"].atts["cool"] = 13;
	map["test"].atts["quote"] = 100;

	stat.CopyRel("test", "new");
	EXPECT_EQ(5, map["new"].numTuples);
	EXPECT_EQ(10, map["new"].atts["some"]);
	EXPECT_EQ(13, map["new"].atts["cool"]);
	EXPECT_EQ(100, map["new"].atts["quote"]);
}

/**
 * CopyRel should throw a out_of_range exception if an attempt to copy from a non-existing relation
 * occurs
 */
TEST_F(StatisticsTest, CopyRel2) {
	ASSERT_THROW(stat.CopyRel("test", "new"), std::out_of_range);
}

/**
 * CopyRel should not fail if there are no attributes in the old relation
 */
TEST_F(StatisticsTest, CopyRel3) {
	map["test"].numTuples = 5;
	stat.CopyRel("test", "new");
	EXPECT_EQ(5, map["new"].numTuples);
}
