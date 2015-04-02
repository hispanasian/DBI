#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * AddAtt should add the attribute with the given name to the map
 */
TEST_F(StatisticsTest, AddAtt1) {
	// Create relations
	map["test1"];
	map["test2"];
	map["test3"];
	map["test4"];

	stat.AddAtt("test1", "bob", 5);
	stat.AddAtt("test2", "alice", 15);
	stat.AddAtt("test3", "great", 25);
	stat.AddAtt("test4", "scott", 13);
	stat.AddAtt("test4", "batman", 69);

	EXPECT_EQ(map["test1"].atts["bob"], 5);
	EXPECT_EQ(map["test2"].atts["alice"], 15);
	EXPECT_EQ(map["test3"].atts["great"], 25);
	EXPECT_EQ(map["test4"].atts["scott"], 13);
	EXPECT_EQ(map["test4"].atts["batman"], 69);
}

/**
 * AddAtt should replace existing relations with the latest relation
 */
TEST_F(StatisticsTest, AddAtt2) {
	// Create relations
	map["test1"];
	map["test2"];
	map["test3"];
	map["test4"];

	stat.AddAtt("test1", "bob", 10);
	stat.AddAtt("test1", "bob", 5);
	stat.AddAtt("test2", "alice", 15);
	stat.AddAtt("test3", "great", 25);
	stat.AddAtt("test4", "scott", 654654);
	stat.AddAtt("test4", "scott", 13);
	stat.AddAtt("test4", "batman", 69);

	EXPECT_EQ(map["test1"].atts["bob"], 5);
	EXPECT_EQ(map["test2"].atts["alice"], 15);
	EXPECT_EQ(map["test3"].atts["great"], 25);
	EXPECT_EQ(map["test4"].atts["scott"], 13);
	EXPECT_EQ(map["test4"].atts["batman"], 69);
}

/**
 * AddAtt should throw a out_of_range if it an attempt is made to add to a non-existing relation
 */
TEST_F(StatisticsTest, AddAtt3) {
	ASSERT_THROW(stat.AddAtt("test", "onetwo", 0), std::out_of_range);
}
