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
 * 1 + 1 + 1 = 3
 * 1 * 1 * 1 = 1
 * 3 - 1/1 = 2
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
 * 1 + 2 + 3 = 6
 * 3 * 2 * 1 = 6
 * 6 - 3/6 = 5.5
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


/**
 * Should combine only the expressions
 * which will combine, regardless of their positions
 * in the list of expressions
 * Elements 2 and 4 will combine, the rest will not
 */
// TEST_F(StatisticsTest, CombineExpressions4) {
// 	StrictMock<MockExpression> mock1;
// 	StrictMock<MockExpression> mock2;
// 	StrictMock<MockExpression> mock3;
// 	StrictMock<MockExpression> mock4;
// 	StrictMock<MockExpression> mock5;
// 	vector<Expression*> exps;
// 	exps.push_back(&mock1);
// 	exps.push_back(&mock2);
// 	exps.push_back(&mock3);
// 	exps.push_back(&mock4);
// 	exps.push_back(&mock5);

// 	Sequence s;
// 	EXPECT_CALL(mock1, Combine(_)).
// 		WillRepeatedly(Return(false));
// 	EXPECT_CALL(mock2, Combine(_)).
// 		InSequence(s).
// 		WillOnce(Return(false));
// 	EXPECT_CALL(mock2, Combine(_)).
// 		InSequence(s).
// 		WillOnce(Return(true));
// 	EXPECT_CALL(mock2, Combine(_)).
// 		InSequence(s).
// 		WillOnce(Return(false));
// 	EXPECT_CALL(mock3, Combine(_)).
// 		WillRepeatedly(Return(false));
// 	EXPECT_CALL(mock4, Combine(_)).
// 		WillRepeatedly(Return(false));
// 	EXPECT_CALL(mock5, Combine(_)).
// 		WillRepeatedly(Return(false));

// 	stat.CombineExpressions(exps);	
// 	EXPECT_EQ(4, exps.size());
// }

