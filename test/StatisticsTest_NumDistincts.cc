#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * NumDistincts should return the number of distinct tuples of the existing relation and attribute
 */
TEST_F(StatisticsTest, NumDistincts1) {
	map["test1"].atts["hey"] = 5;
	EXPECT_EQ(5, stat.NumDistincts("test1", "hey"));
}

/**
 * NumDistincts should return -1 if the relation does not exist and it should not create the
 * relation.
 */
TEST_F(StatisticsTest, NumDistincts2) {
	EXPECT_EQ(-1, stat.NumDistincts("keanu", "reaves"));
	ASSERT_THROW(map.at("test"), std::out_of_range);
}

/**
 * NumDistincts should return -1 if the attribute does not exist and it should not create the
 * relation
 */
TEST_F(StatisticsTest, NumDistincts3) {
	map["bob"];
	EXPECT_EQ(-1, stat.NumDistincts("bob", "dylan"));
	ASSERT_THROW(map.at("test").atts.at("dylan"), std::out_of_range);
}
