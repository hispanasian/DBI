#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * AddRel should add the relation with the given name to the map
 */
TEST_F(StatisticsTest, AddRel1) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);
	stat.AddRel("test3", 25);
	stat.AddRel("test4", 13);
	stat.AddRel("test5", 69);

	EXPECT_EQ(map["test1"].numTuples, 5);
	EXPECT_EQ(map["test2"].numTuples, 15);
	EXPECT_EQ(map["test3"].numTuples, 25);
	EXPECT_EQ(map["test4"].numTuples, 13);
	EXPECT_EQ(map["test5"].numTuples, 69);
}

/**
 * AddRel should replace existing relations with the latest relation
 */
TEST_F(StatisticsTest, AddRel2) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);
	stat.AddRel("test3", 25);
	stat.AddRel("test4", 50);
	stat.AddRel("test4", 13);
	stat.AddRel("test5", 97);
	stat.AddRel("test5", 69);

	EXPECT_EQ(map["test1"].numTuples, 5);
	EXPECT_EQ(map["test2"].numTuples, 15);
	EXPECT_EQ(map["test3"].numTuples, 25);
	EXPECT_EQ(map["test4"].numTuples, 13);
	EXPECT_EQ(map["test5"].numTuples, 69);
}
