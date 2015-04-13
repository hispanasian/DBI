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

	EXPECT_EQ(5, map["test1"].numTuples);
	EXPECT_EQ(15, map["test2"].numTuples);
	EXPECT_EQ(25, map["test3"].numTuples);
	EXPECT_EQ(13, map["test4"].numTuples);
	EXPECT_EQ(69, map["test5"].numTuples);
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

	EXPECT_EQ(5, map["test1"].numTuples);
	EXPECT_EQ(15, map["test2"].numTuples);
	EXPECT_EQ(25, map["test3"].numTuples);
	EXPECT_EQ(13, map["test4"].numTuples);
	EXPECT_EQ(69, map["test5"].numTuples);
}

/**
 * AddRel should update the numTuples of every relation in its set
 */
TEST_F(StatisticsTest, AddRel3) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);

	map["test1"].set.insert("test2");
	map["test2"].set.insert("test1");

	stat.AddRel("test1", 20);


	EXPECT_EQ(20, map["test1"].numTuples);
	EXPECT_EQ(20, map["test2"].numTuples);
}

/**
 * AddRel should, if it did not exist, add itself to its own set.
 */
TEST_F(StatisticsTest, AddRel4) {
	stat.AddRel("test1", 5);

	EXPECT_EQ(1, map["test1"].set.count("test1"));
}
