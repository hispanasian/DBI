#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * VerifyJoinSets should return true if every relation belongs (completely) to the same set(s)
 */
TEST_F(StatisticsTest, VerifyJoinSets1) {
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddRel("rel3", 0);
	stat.AddRel("rel4", 0);
	stat.AddRel("rel5", 0);

	stat.MergeSets("rel1", "rel2");
	stat.MergeSets("rel1", "rel3");
	stat.MergeSets("rel1", "rel4");

	const char *relNames[] = { "rel1", "rel2", "rel3", "rel4", "rel5" };

	EXPECT_EQ(true, stat.VerifyJoinSets(relNames, 5));
}

/**
 * CopyRel should return false if the relations in relNames are missing a member in their sets.
 */
TEST_F(StatisticsTest, VerifyJoinSets2) {
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddRel("rel3", 0);
	stat.AddRel("rel4", 0);
	stat.AddRel("rel5", 0);

	stat.MergeSets("rel1", "rel2");
	stat.MergeSets("rel1", "rel3");
	stat.MergeSets("rel1", "rel4");

	const char *relNames[] = { "rel1", "rel2", "rel3", "rel5" };

	EXPECT_EQ(false, stat.VerifyJoinSets(relNames, 4));
}

/**
 * Another case where VerifyJoinSets should return true
 */
TEST_F(StatisticsTest, VerifyJoinSets3) {
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddRel("rel3", 0);
	stat.AddRel("rel4", 0);
	stat.AddRel("rel5", 0);

	const char *relNames[] = { "rel1", "rel2", "rel3", "rel4", "rel5" };

	EXPECT_EQ(true, stat.VerifyJoinSets(relNames, 5));
}

/**
 * Another case where VerifyJoinSets should return true
 */
TEST_F(StatisticsTest, VerifyJoinSets4) {
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddRel("rel3", 0);
	stat.AddRel("rel4", 0);
	stat.AddRel("rel5", 0);

	stat.MergeSets("rel1", "rel2");
	stat.MergeSets("rel1", "rel3");
	stat.MergeSets("rel4", "rel5");

	const char *relNames[] = { "rel1", "rel2", "rel3", "rel4", "rel5" };

	EXPECT_EQ(true, stat.VerifyJoinSets(relNames, 5));
}
