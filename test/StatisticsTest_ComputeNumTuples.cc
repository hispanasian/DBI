#include "StatisticsTest.h"

/**
 * Should return the value of NumTuples() of a single expression
 */
TEST_F(StatisticsTest, ComputeNumTuples1) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);

	EXPECT_CALL(mock, Tuples()).
		WillOnce(Return(10.0));

	EXPECT_EQ(10.0, stat.ComputeNumTuples(exps));
}

/**
 * Should compute the number of tuples according to the formula
 * sum -> 1 + 1 + 1 = 3
 * denominator -> 1 * 1 * 1 = 1
 * numerator -> 1
 * result -> 3 - 1/1 = 2
 */
TEST_F(StatisticsTest, ComputeNumTuples2) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);
	exps.push_back(&mock);
	exps.push_back(&mock);

	EXPECT_CALL(mock, Tuples()).
		Times(3).
		WillRepeatedly(Return(1));
	EXPECT_CALL(mock, Numerator()).
		WillRepeatedly(Return(1));
	EXPECT_CALL(mock, Denominator()).
		WillRepeatedly(Return(1));

	EXPECT_EQ(2.0, stat.ComputeNumTuples(exps));
}

/**
 * Should compute the number of tuples according to the formula
 * sum -> 1 + 2 + 3 = 6
 * denominator -> 3 * 2 * 1 = 6
 * numerator -> 3
 * result -> 6 - 3/6 = 5.5
 */
TEST_F(StatisticsTest, ComputeNumTuples3) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);
	exps.push_back(&mock);
	exps.push_back(&mock);

	Sequence s1;
	Sequence s2;
	EXPECT_CALL(mock, Tuples()).
		InSequence(s1).
		WillOnce(Return(1));
	EXPECT_CALL(mock, Tuples()).
		InSequence(s1).
		WillOnce(Return(2));
	EXPECT_CALL(mock, Tuples()).
		InSequence(s1).
		WillOnce(Return(3));
	EXPECT_CALL(mock, Denominator()).
		InSequence(s2).
		WillOnce(Return(3));
	EXPECT_CALL(mock, Denominator()).
		InSequence(s2).
		WillOnce(Return(2));
	EXPECT_CALL(mock, Denominator()).
		InSequence(s2).
		WillOnce(Return(1));
	EXPECT_CALL(mock, Numerator()).
		WillRepeatedly(Return(3));
	
	EXPECT_EQ(5.5, stat.ComputeNumTuples(exps));
}
