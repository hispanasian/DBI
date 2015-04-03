#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * The copy constructor should populate the new relation with the old relations data
 */
TEST_F(StatisticsTest, Constructor1) {
	// Add Relations
	map["rel1"].numTuples = 1;
	map["rel2"].numTuples = 2;
	map["rel3"].numTuples = 3;
	map["rel4"].numTuples = 4;

	// Add Attributes
	map["rel1"].atts["att1"] = 1;
	map["rel1"].atts["att2"] = 2;
	map["rel2"].atts["att3"] = 3;
	map["rel2"].atts["att4"] = 4;
	map["rel3"].atts["att5"] = 5;
	map["rel3"].atts["att6"] = 6;
	map["rel3"].atts["att7"] = 7;
	map["rel3"].atts["att8"] = 8;
	map["rel3"].atts["att9"] = 9;

	Statistics copy = Statistics(stat);

	EXPECT_EQ(copy.NumTuples("rel1"), 1);
	EXPECT_EQ(copy.NumTuples("rel2"), 2);
	EXPECT_EQ(copy.NumTuples("rel3"), 3);
	EXPECT_EQ(copy.NumTuples("rel4"), 4);

	EXPECT_EQ(copy.NumDistincts("rel1", "att1"), 1);
	EXPECT_EQ(copy.NumDistincts("rel1", "att2"), 2);
	EXPECT_EQ(copy.NumDistincts("rel2", "att3"), 3);
	EXPECT_EQ(copy.NumDistincts("rel2", "att4"), 4);
	EXPECT_EQ(copy.NumDistincts("rel3", "att5"), 5);
	EXPECT_EQ(copy.NumDistincts("rel3", "att6"), 6);
	EXPECT_EQ(copy.NumDistincts("rel3", "att7"), 7);
	EXPECT_EQ(copy.NumDistincts("rel3", "att8"), 8);
	EXPECT_EQ(copy.NumDistincts("rel3", "att9"), 9);
}

/**
 * The copy relation should not fail if the old relation is empty
 */
TEST_F(StatisticsTest, Constructor2) {
	Statistics copy = Statistics(stat);
}

/**
 * The copy relation should not fail if the old relation has no attributes
 */
TEST_F(StatisticsTest, Constructor3) {
	// Add Relations
	map["rel1"].numTuples = 1;
	map["rel2"].numTuples = 2;
	map["rel3"].numTuples = 3;
	map["rel4"].numTuples = 4;

	Statistics copy = Statistics(stat);
	EXPECT_EQ(copy.NumTuples("rel1"), 1);
	EXPECT_EQ(copy.NumTuples("rel2"), 2);
	EXPECT_EQ(copy.NumTuples("rel3"), 3);
	EXPECT_EQ(copy.NumTuples("rel4"), 4);
}
