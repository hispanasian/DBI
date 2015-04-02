#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * NumTuples should return the number of tuples in an existing relation if it exists.
 */
TEST_F(StatisticsTest, NumTuples1) {
	map["test"].numTuples = 5;

	EXPECT_EQ(5, stat.NumTuples("test"));
}

/**
 * NumTuples should return null if no relation with the provided name exists and it should not
 * create an object in the map.
 */
TEST_F(StatisticsTest, NumTuples2) {
	EXPECT_EQ(NULL, stat.NumTuples("test"));
	ASSERT_THROW(map.at("test"), std::out_of_range);
}
