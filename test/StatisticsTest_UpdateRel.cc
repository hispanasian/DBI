#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * UpdateRel should add the relation with the given name to the map
 */
TEST_F(StatisticsTest, UpdateRel1) {
	UpdateRel("test1", 5);
	UpdateRel("test2", 15);
	UpdateRel("test3", 25);
	UpdateRel("test4", 13);
	UpdateRel("test5", 69);

	EXPECT_EQ(map["test1"].numTuples, 5);
	EXPECT_EQ(map["test2"].numTuples, 15);
	EXPECT_EQ(map["test3"].numTuples, 25);
	EXPECT_EQ(map["test4"].numTuples, 13);
	EXPECT_EQ(map["test5"].numTuples, 69);
}

/**
 * UpdateRel should replace existing relations with the latest relation
 */
TEST_F(StatisticsTest, UpdateRel2) {
	UpdateRel("test1", 5);
	UpdateRel("test2", 15);
	UpdateRel("test3", 25);
	UpdateRel("test4", 50);
	UpdateRel("test4", 13);
	UpdateRel("test5", 97);
	UpdateRel("test5", 69);

	EXPECT_EQ(map["test1"].numTuples, 5);
	EXPECT_EQ(map["test2"].numTuples, 15);
	EXPECT_EQ(map["test3"].numTuples, 25);
	EXPECT_EQ(map["test4"].numTuples, 13);
	EXPECT_EQ(map["test5"].numTuples, 69);
}
