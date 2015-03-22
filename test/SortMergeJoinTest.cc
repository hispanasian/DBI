#include "SortMergeJoinTest.h"


// All records on left are > those on right
TEST_F(SortMergeJoinTest, AlignGroups1) {
	Sequence s2;
	EXPECT_CALL(inR, Remove(&tempR)).
		Times(3).
		InSequence(s2).
		WillRepeatedly(Return(1));
	EXPECT_CALL(inR, Remove(&tempR)).
		InSequence(s2).
		WillOnce(Return(0));

	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		WillRepeatedly(Return(1));


	EXPECT_EQ(true, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}

// All records on left are < those on right
TEST_F(SortMergeJoinTest, AlignGroups2) {
	Sequence s1;
	EXPECT_CALL(inL, Remove(&tempL)).
		Times(3).
		InSequence(s1).
		WillRepeatedly(Return(1));
	EXPECT_CALL(inL, Remove(&tempL)).
		InSequence(s1).
		WillOnce(Return(0));

	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		WillRepeatedly(Return(-1));


	EXPECT_EQ(true, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}

// The the given records match
TEST_F(SortMergeJoinTest, AlignGroups3) {
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		WillRepeatedly(Return(0));

	EXPECT_EQ(false, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}

// A match is found after advancing the left
TEST_F(SortMergeJoinTest, AlignGroups4) {
	EXPECT_CALL(inL, Remove(&tempL)).
		Times(3).
		WillRepeatedly(Return(1));

	Sequence s1;
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		Times(3).
		InSequence(s1).
		WillRepeatedly(Return(-1));
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		InSequence(s1).
		WillOnce(Return(0));


	EXPECT_EQ(false, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}

// A match is found after advancing the right
TEST_F(SortMergeJoinTest, AlignGroups5) {
	EXPECT_CALL(inR, Remove(&tempR)).
		Times(3).
		WillRepeatedly(Return(1));

	Sequence s1;
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		Times(3).
		InSequence(s1).
		WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		InSequence(s1).
		WillOnce(Return(0));


	EXPECT_EQ(false, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}
