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

// It throws exceptions when ops with two 
// literals are given

