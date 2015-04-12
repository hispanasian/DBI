#include "StatisticsTest.h"

/**
 * Should leave a single expression alone
 */
TEST_F(StatisticsTest, CombineExpressions1) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);
	stat.CombineExpressions(exps);
	EXPECT_EQ(1, exps.size());
}

/**
 * Should leave all expressions intact
 * if none of them combine
 */
TEST_F(StatisticsTest, CombineExpressions2) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);
	exps.push_back(&mock);
	exps.push_back(&mock);

	EXPECT_CALL(mock, Combine(Ref(mock))).
		Times(3).
		WillRepeatedly(Return(false));

	stat.CombineExpressions(exps);	
	EXPECT_EQ(3, exps.size());
}

/**
 * Should combine all expressions to 1 expression
 * if all of them combine
 */
TEST_F(StatisticsTest, CombineExpressions3) {
	StrictMock<MockExpression> mock;
	vector<Expression*> exps;
	exps.push_back(&mock);
	exps.push_back(&mock);
	exps.push_back(&mock);

	EXPECT_CALL(mock, Combine(Ref(mock))).
		Times(2).
		WillRepeatedly(Return(true));

	stat.CombineExpressions(exps);	
	EXPECT_EQ(1, exps.size());
}


/**
 * Should combine only the expressions
 * which will combine, regardless of their positions
 * in the list of expressions
 * Elements 2 and 4 will combine, the rest will not
 */
TEST_F(StatisticsTest, CombineExpressions4) {
	StrictMock<MockExpression> mock1;
	StrictMock<MockExpression> mock2;
	StrictMock<MockExpression> mock3;
	StrictMock<MockExpression> mock4;
	StrictMock<MockExpression> mock5;
	vector<Expression*> exps;
	exps.push_back(&mock1);
	exps.push_back(&mock2);
	exps.push_back(&mock3);
	exps.push_back(&mock4);
	exps.push_back(&mock5);

	Sequence s;
	EXPECT_CALL(mock1, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(false));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(true));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(false));
	EXPECT_CALL(mock3, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock4, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock5, Combine(_)).
		WillRepeatedly(Return(false));

	stat.CombineExpressions(exps);	
	EXPECT_EQ(4, exps.size());
}
