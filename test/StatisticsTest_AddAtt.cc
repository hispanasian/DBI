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
 * AddAtt should update the lookup table when a new attribute is added
 */
TEST_F(StatisticsTest, AddAtt4) {
	ASSERT_THROW(stat.AddAtt("test", "onetwo", 0), std::out_of_range);
}

/**
 * AddAtt should replace existing relations with the latest relation
 */
TEST_F(StatisticsTest, AddAtt5) {
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

	string test1 = string("test1");
	string test2 = string("test2");
	string test3 = string("test3");
	string test4 = string("test4");

	EXPECT_EQ(0 , test1.compare(stat.RelLookup("bob")));
	EXPECT_EQ(0 , test2.compare(stat.RelLookup("alice")));
	EXPECT_EQ(0 , test3.compare(stat.RelLookup("great")));
	EXPECT_EQ(0 , test4.compare(stat.RelLookup("scott")));
	EXPECT_EQ(0 , test4.compare(stat.RelLookup("batman")));
}
