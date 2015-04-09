#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * ParseOperand should return true when only the attribute is provided and it exists in Statistics.
 */
TEST_F(StatisticsTest, ParseOperand1) {
	string operand = "att";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 0);
	vector<string> out;

	ASSERT_EQ(true, stat.ParseOperand(operand, out));
	EXPECT_EQ(0, out[0].compare("rel"));
	EXPECT_EQ(0, out[1].compare("att"));
}

/**
 * ParseOperand should return true when the relation and attribute are provided and it exists in
 * Statistics.
 */
TEST_F(StatisticsTest, ParseOperand2) {
	string operand = "temp.att";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 1);
	stat.CopyRel("rel", "temp");

	vector<string> out;

	ASSERT_EQ(true, stat.ParseOperand(operand, out));
	EXPECT_EQ(0, out[0].compare("temp"));
	EXPECT_EQ(0, out[1].compare("att"));
}

/**
 * ParseOperand should return false if the attribute in the operand does not exist.
 */
TEST_F(StatisticsTest, ParseOperand3) {
	string operand = "nothing";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 0);
	vector<string> out;

	EXPECT_EQ(false, stat.ParseOperand(operand, out));
}

/**
 * ParseOperand should return false if the attribute in the operand does not exist even if the
 * relation is provided (ie, it verifies that the relation does in fact exist).
 */
TEST_F(StatisticsTest, ParseOperand4) {
	string operand = "nothing.att";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 0);
	vector<string> out;

	EXPECT_EQ(false, stat.ParseOperand(operand, out));
}

/**
 * ParseOperand should return false if the attribute in the operand does not exist even if the
 * relation is provided (ie, it verifies that the attribute belongs to the relation).
 */
TEST_F(StatisticsTest, ParseOperand5) {
	string operand = "rel.nothing";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 0);
	vector<string> out;

	EXPECT_EQ(false, stat.ParseOperand(operand, out));
}
