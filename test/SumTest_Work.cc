#include "SumTest.h"

/*
 * Work will sum up all the integers
 */

TEST_F(SumTest, Work1) {
	MakeInt(fun);
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(1), Return(Int)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(2), Return(Int)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(3), Return(Int)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(4), Return(Int)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(5), Return(Int)));

	// Arbitrary
	EXPECT_CALL(fun, ReturnsInt()).
			WillRepeatedly(Return(true));

	op.Work(in, out, fun, r);

	Record result;
	out.Remove(&r);
	EXPECT_EQ(15, GetInt(result));
}

/*
 * Work will sum up all the doubles
 */

TEST_F(SumTest, Work2) {
	MakeDouble(fun);
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<2>(0.1), Return(Double)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<2>(0.2), Return(Double)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<2>(0.3), Return(Double)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<2>(0.4), Return(Double)));
	EXPECT_CALL(fun, Apply(Ref(r), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<2>(0.5), Return(Double)));

	// Arbitrary
	EXPECT_CALL(fun, ReturnsInt()).
			WillRepeatedly(Return(false));

	op.Work(in, out, fun, r);

	Record result;
	out.Remove(&r);
	EXPECT_EQ(1.5, GetDouble(result));
}

/**
 * Work should be able to handle an empty input queue
 */
TEST_F(SumTest, Work3) {
	MakeInt(fun);
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	// Arbitrary
	EXPECT_CALL(fun, ReturnsInt()).
			WillRepeatedly(Return(true));

	op.Work(in, out, fun, r);

	Record result;
	out.Remove(&r);
	EXPECT_EQ(0, GetInt(result));
}

/**
 * Work should be able to handle an empty input queue
 */
TEST_F(SumTest, Work4) {
	MakeDouble(fun);
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	// Arbitrary
	EXPECT_CALL(fun, ReturnsInt()).
			WillRepeatedly(Return(false));

	op.Work(in, out, fun, r);

	Record result;
	out.Remove(&r);
	EXPECT_EQ(0, GetInt(result));
}
