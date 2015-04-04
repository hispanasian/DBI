#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Statistics.h"
#include "Expression.h"
#include "MockClasses.h"
#include "ParseTree.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

/**
 * Binary Expression should not combine with a UnaryExpression
 */
TEST(BinaryExpressionTest, Combine1) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	BinaryExpression exp1 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);
	UnaryExpression exp2 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);

	EXPECT_EQ(false, exp1.Combine(exp2));
}

/**
 * Binary Expression should only combine with a identical Binary Expression
 */
TEST(BinaryExpressionTest, Combine2) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	BinaryExpression exp1 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);
	BinaryExpression exp2 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);

	EXPECT_EQ(true, exp1.Combine(exp2));
}

/**
 * Binary Expression should not combine with another BinaryExpression that only differs in operator
 */
TEST(BinaryExpressionTest, Combine3) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	BinaryExpression exp1 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", LESS_THAN);
	BinaryExpression exp2 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);

	EXPECT_EQ(false, exp1.Combine(exp2));
}

/**
 * Binary Expression should combine with a identical Binary Expression, the "position" of the
 * relation doe snot matter
 */
TEST(BinaryExpressionTest, Combine4) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	BinaryExpression exp1 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);
	BinaryExpression exp2 = BinaryExpression(stat, "rel2", "att2", "rel1", "att1", EQUALS);

	EXPECT_EQ(true, exp1.Combine(exp2));
}

/**
 * UnaryExpression should not combine with BinaryExpression
 */
TEST(UnaryExpressionTest, Combine1) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "5", EQUALS);
	BinaryExpression exp2 = BinaryExpression(stat, "rel1", "att1", "rel2", "att2", EQUALS);

	EXPECT_EQ(false, exp1.Combine(exp2));
	EXPECT_EQ(1, exp1.Count());
}

/**
 * UnaryExpression should only combine with another UnaryExpression that shares attribute and
 * relation
 */
TEST(UnaryExpressionTest, Combine2) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);
	UnaryExpression exp2 = UnaryExpression(stat, "rel1", "att1", "2", EQUALS);

	EXPECT_EQ(true, exp1.Combine(exp2));
	EXPECT_EQ(2, exp1.Count());
}

/**
 * UnaryExpression should not combine with another UnaryExpression that differs only in relation
 */
TEST(UnaryExpressionTest, Combine3) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);
	UnaryExpression exp2 = UnaryExpression(stat, "rel2", "att1", "4", EQUALS);

	EXPECT_EQ(false, exp1.Combine(exp2));
	EXPECT_EQ(1, exp1.Count());
}

/**
 * UnaryExpression should not combine with another UnaryExpression that only differs in operator
 */
TEST(UnaryExpressionTest, Combine4) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);
	UnaryExpression exp2 = UnaryExpression(stat, "rel1", "att1", "4", LESS_THAN);

	EXPECT_EQ(false, exp1.Combine(exp2));
	EXPECT_EQ(1, exp1.Count());
}

/**
 * UnaryExpression should not add identical UnaryExpressions to the literal count, but should still
 * return true.
 */
TEST(UnaryExpressionTest, Combine5) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);
	UnaryExpression exp2 = UnaryExpression(stat, "rel1", "att1", "4", EQUALS);

	EXPECT_EQ(true, exp1.Combine(exp2));
	EXPECT_EQ(1, exp1.Count());
}

/**
 * UnaryExpression should return true even when the operator is not an equals operator but both
 * expressions are identical.
 */
TEST(UnaryExpressionTest, Combine6) {
	Statistics stat;
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddAtt("rel1", "att1", 0);
	stat.AddAtt("rel2", "att2", 0);

	UnaryExpression exp1 = UnaryExpression(stat, "rel1", "att1", "4", LESS_THAN);
	UnaryExpression exp2 = UnaryExpression(stat, "rel1", "att1", "4", LESS_THAN);

	EXPECT_EQ(true, exp1.Combine(exp2));
	EXPECT_EQ(1, exp1.Count());
}
