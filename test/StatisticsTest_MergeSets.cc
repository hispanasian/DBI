#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * MergeSets should merge the provided the sets containing each relation.
 */
TEST_F(StatisticsTest, MergeSets1) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);
	stat.AddRel("test3", 25);
	stat.AddRel("test4", 13);
	stat.AddRel("test5", 69);

	stat.MergeSets("test1", "test2");
	stat.MergeSets("test3", "test4");
	stat.MergeSets("test4", "test5");

	set<string> set1 = stat.GetSet("test1"); // test1, test2
	set<string> set2 = stat.GetSet("test3"); // test3, test4, test5

	EXPECT_EQ(1, set1.count("test1"));
	EXPECT_EQ(1, set1.count("test2"));
	EXPECT_EQ(0, set1.count("test3"));
	EXPECT_EQ(0, set1.count("test4"));
	EXPECT_EQ(0, set1.count("test5"));

	EXPECT_EQ(0, set2.count("test1"));
	EXPECT_EQ(0, set2.count("test2"));
	EXPECT_EQ(1, set2.count("test3"));
	EXPECT_EQ(1, set2.count("test4"));
	EXPECT_EQ(1, set2.count("test5"));

	// Final set
	stat.MergeSets("test2", "test4");
	set<string> set = stat.GetSet("test5"); // all tests
	EXPECT_EQ(1, set.count("test1"));
	EXPECT_EQ(1, set.count("test2"));
	EXPECT_EQ(1, set.count("test3"));
	EXPECT_EQ(1, set.count("test4"));
	EXPECT_EQ(1, set.count("test5"));
}

/**
 * MergeSets should do nothing to a set if both relations exist in the same set
 */
TEST_F(StatisticsTest, MergeSets2) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);

	stat.MergeSets("test1", "test2");

	set<string> set1 = stat.GetSet("test1"); // test1, test2
	set<string> set2 = stat.GetSet("test2"); // test3, test4, test5

	EXPECT_EQ(1, set1.count("test1"));
	EXPECT_EQ(1, set1.count("test2"));
	EXPECT_EQ(1, set2.count("test1"));
	EXPECT_EQ(1, set2.count("test2"));

	stat.MergeSets("test1", "test2");
	set<string> set3 = stat.GetSet("test1"); // test1, test2
	set<string> set4 = stat.GetSet("test2"); // test3, test4, test5
	EXPECT_EQ(1, set3.count("test1"));
	EXPECT_EQ(1, set3.count("test2"));
	EXPECT_EQ(1, set4.count("test1"));
	EXPECT_EQ(1, set4.count("test2"));
}
