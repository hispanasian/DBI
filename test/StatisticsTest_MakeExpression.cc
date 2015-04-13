#include "StatisticsTest.h"

// It can parse Unary ops
TEST_F(StatisticsTest, MakeExpression1) {
	Operand left {INT, "4"};
	Operand right {NAME, "attribute"};
	ComparisonOp op {LESS_THAN, &left, &right};
	vector<Expression*> expressions;
	set<string> relations;

	stat.AddRel("relation", 100);
	stat.AddAtt("relation", "attribute", 10);

	stat.MakeExpression(op, expressions, relations);
	ASSERT_EQ(1, expressions.size());
	EXPECT_EQ(UNARY, expressions[0]->type);
	EXPECT_EQ(LESS_THAN, expressions[0]->op);
	UnaryExpression* ue = dynamic_cast<UnaryExpression*> (expressions[0]);
	EXPECT_STREQ("relation", GetRel(*ue).c_str());
	EXPECT_STREQ("attribute", GetAtt(*ue).c_str());
	ASSERT_EQ(1, GetLits(*ue).size());
	auto it = GetLits(*ue).begin();
	EXPECT_STREQ("4", it->c_str());
	ASSERT_EQ(1, relations.size());
	it = relations.begin();
	ASSERT_EQ("relation", *it);
}

// It can parse Binary ops
TEST_F(StatisticsTest, MakeExpression2) {
	Operand left {NAME, "attLeft"};
	Operand right {NAME, "attRight"};
	ComparisonOp op {EQUALS, &left, &right};
	vector<Expression*> expressions;
	set<string> relations;

	stat.AddRel("relLeft", 100);
	stat.AddAtt("relLeft", "attLeft", 10);
	stat.AddRel("relRight", 100);
	stat.AddAtt("relRight", "attRight", 10);

	stat.MakeExpression(op, expressions, relations);
	ASSERT_EQ(1, expressions.size());
	EXPECT_EQ(BINARY, expressions[0]->type);
	EXPECT_EQ(EQUALS, expressions[0]->op);
	BinaryExpression* be = dynamic_cast<BinaryExpression*> (expressions[0]);
	EXPECT_STREQ("relLeft", GetRel1(*be).c_str());
	EXPECT_STREQ("attLeft", GetAtt1(*be).c_str());
	EXPECT_STREQ("relRight", GetRel2(*be).c_str());
	EXPECT_STREQ("attRight", GetAtt2(*be).c_str());
	ASSERT_EQ(2, relations.size());
	auto it = relations.begin();
	ASSERT_EQ("relLeft", *it);
	it++;
	ASSERT_EQ("relRight", *it);
}

// It throws exceptions when ops with two 
// literals are given
TEST_F(StatisticsTest, MakeExpression3) {
	Operand left {INT, "1"};
	Operand right {INT, "2"};
	ComparisonOp op {EQUALS, &left, &right};
	vector<Expression*> expressions;
	set<string> relations;

	EXPECT_THROW(stat.MakeExpression(op, expressions, relations), std::runtime_error);
}