#include "GroupByTest.h"

/*
	GroupBy will insert nothing into the output
	pipe if there are no records in the input pipe.
*/
TEST_F(GroupByTest, Work1) {
	EXPECT_CALL(in, Remove(&rec)).
			WillOnce(Return(0));

	op.Work(in, out, groupAtts, func, rec, prev, comp);
	EXPECT_EQ(0, out.Remove(&rec));
}

/*
	GroupBy will compute the sum for a single record
	with an int sum type
*/
TEST_F(GroupByTest, Work2) {
	MakeInt(func);
	Sequence s1;
	EXPECT_CALL(in, Remove(&rec)).
			Times(1).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&rec)).
			InSequence(s1).
			WillOnce(Return(0));

	EXPECT_CALL(prev, Copy(&rec)).
			Times(1);

	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			WillOnce(DoAll(SetArgReferee<1>(1), Return(Int)));
	EXPECT_CALL(func, ReturnsInt()).
		WillRepeatedly(Return(true));

	op.Work(in, out, groupAtts, func, rec, prev, comp);
	Record result;
	EXPECT_EQ(1, out.Remove(&result));
	EXPECT_EQ(1, GetInt(result));
	EXPECT_EQ(0, out.Remove(&result));
}

/*
	GroupBy will compute the sum for a single record
	with a double sum type
*/
TEST_F(GroupByTest, Work3) {
	MakeDouble(func);
	Sequence s1;
	EXPECT_CALL(in, Remove(&rec)).
			Times(1).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&rec)).
			InSequence(s1).
			WillOnce(Return(0));

	EXPECT_CALL(prev, Copy(&rec)).
			Times(1);

	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			WillOnce(DoAll(SetArgReferee<2>(1.5), Return(Double)));
	EXPECT_CALL(func, ReturnsInt()).
		WillRepeatedly(Return(false));

	op.Work(in, out, groupAtts, func, rec, prev, comp);
	Record result;
	EXPECT_EQ(1, out.Remove(&result));
	EXPECT_EQ(1.5, GetDouble(result));
	EXPECT_EQ(0, out.Remove(&result));
}


// All records in same group
TEST_F(GroupByTest, Work4) {
	MakeInt(func);
	Sequence s1;
	EXPECT_CALL(in, Remove(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&rec)).
			InSequence(s1).
			WillOnce(Return(0));


	Sequence s2;
	EXPECT_CALL(prev, Copy(&rec)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &groupAtts)).
			InSequence(s2).
			WillOnce(Return(0));
	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(1), Return(Int)));
	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(2), Return(Int)));
	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(3), Return(Int)));
		EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(4), Return(Int)));
	EXPECT_CALL(func, Apply(Ref(rec), _, _)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<1>(5), Return(Int)));

	Sequence s3;

	
	
	EXPECT_CALL(func, ReturnsInt()).
		WillRepeatedly(Return(true));

	op.Work(in, out, groupAtts, func, rec, prev, comp);
	Record result;
	EXPECT_EQ(1, out.Remove(&result));
	EXPECT_EQ(15, GetInt(result));
	EXPECT_EQ(0, out.Remove(&result));
}

// All records in differnet group
TEST_F(GroupByTest, Work5) {

}

// 4 different groups
TEST_F(GroupByTest, Work6) {

}