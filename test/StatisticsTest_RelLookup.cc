#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * RelLookup should return the appropriate relation provided the appropriate attribute
 */
TEST_F(StatisticsTest, RelLookup1) {
	lookup["test"] = "pass";

	string pass = string("pass");
	EXPECT_EQ(0, pass.compare(stat.RelLookup("test")));
}

/**
 * RelLookup should return if a relation is not mapped to an attribute
 */
TEST_F(StatisticsTest, RelLookup2) {
	string pass = string("");
	EXPECT_EQ(0, pass.compare(stat.RelLookup("test")));
}
