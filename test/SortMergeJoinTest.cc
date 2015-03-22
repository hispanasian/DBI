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

// The right pipe is empty
TEST_F(SortMergeJoinTest, InitRightGroup1) {
	EXPECT_CALL(inR, Remove(&tempR)).
		WillOnce(Return(0));

	EXPECT_CALL(relR, Add(&groupRecR)).
		WillOnce(Return(true));

	EXPECT_EQ(true, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next record in the pipe is not part of this group
TEST_F(SortMergeJoinTest, InitRightGroup2) {
	EXPECT_CALL(inR, Remove(&tempR)).
		WillOnce(Return(1));

	EXPECT_CALL(comp, Compare(&groupRecR, &tempR, &orderR)).
		WillOnce(Return(1));

	EXPECT_CALL(relR, Add(&groupRecR)).
		WillOnce(Return(true));

	EXPECT_EQ(false, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next 3 records in the pipe are part of the group,
// but the 4th is not in the group
TEST_F(SortMergeJoinTest, InitRightGroup3) {
	Sequence s1;
	EXPECT_CALL(inR, Remove(&tempR)).
		Times(4).
		InSequence(s1).
		WillRepeatedly(Return(1));

	Sequence s2;
	EXPECT_CALL(comp, Compare(&groupRecR, &tempR, &orderR)).
		Times(3).
		InSequence(s2).
		WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(&groupRecR, &tempR, &orderR)).
		InSequence(s2).
		WillOnce(Return(1));

	EXPECT_CALL(relR, Add(&groupRecR)).	
		WillOnce(Return(true));

	EXPECT_CALL(relR, Add(&tempR)).
		Times(3).
		WillRepeatedly(Return(true));

	EXPECT_EQ(false, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next 3 records in the pipe are part of the group,
// and they are the final records in the pipe
TEST_F(SortMergeJoinTest, InitRightGroup4) {
	Sequence s1;
	EXPECT_CALL(inR, Remove(&tempR)).
		Times(3).
		InSequence(s1).
		WillRepeatedly(Return(1));
	EXPECT_CALL(inR, Remove(&tempR)).
		InSequence(s1).
		WillOnce(Return(0));
	

	EXPECT_CALL(comp, Compare(&groupRecR, &tempR, &orderR)).
		Times(3).
		WillRepeatedly(Return(0));

	EXPECT_CALL(relR, Add(&groupRecR)).	
		WillOnce(Return(true));

	EXPECT_CALL(relR, Add(&tempR)).
		Times(3).
		WillRepeatedly(Return(true));

	EXPECT_EQ(true, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The left pipe is empty
TEST_F(SortMergeJoinTest, StreamLeftGroup1) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		WillOnce(Return(0));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(1);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}

// The next record in the pipe is not part of this group
TEST_F(SortMergeJoinTest, StreamLeftGroup2) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		WillOnce(Return(1));

	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		WillOnce(Return(1));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(1);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// and they all fit in memory
// but the 5th is not in the group
TEST_F(SortMergeJoinTest, StreamLeftGroup3) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		Times(5).
		WillRepeatedly(Return(1));

	Sequence s1;
	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		Times(4).
		InSequence(s1).
		WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		InSequence(s1).
		WillOnce(Return(1));

	EXPECT_CALL(relL, Add(&tempL)).
		Times(4).
		WillRepeatedly(Return(true));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(1);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// but only 2 at a time fit in memory,
// and the 5th is not in the group
TEST_F(SortMergeJoinTest, StreamLeftGroup4) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		Times(5).
		WillRepeatedly(Return(1));

	Sequence s1;
	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		Times(4).
		InSequence(s1).
		WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		InSequence(s1).
		WillOnce(Return(1));

	Sequence s2;
	EXPECT_CALL(relL, Add(&tempL)).
		Times(2).
		InSequence(s2).
		WillRepeatedly(Return(true));
	EXPECT_CALL(relL, Add(&tempL)).
		InSequence(s2).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Add(&tempL)).
		Times(2).
		InSequence(s2).
		WillRepeatedly(Return(true));

	EXPECT_CALL(relL, Clear()).
		Times(1);

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(2);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// and they all fit in memory
// and they are the final records in the pipe
TEST_F(SortMergeJoinTest, StreamLeftGroup5) {
	Sequence s3;
	EXPECT_CALL(inL, Remove(&tempL)).
		Times(4).
		InSequence(s3).
		WillRepeatedly(Return(1));
	EXPECT_CALL(inL, Remove(&tempL)).
		InSequence(s3).
		WillOnce(Return(0));
	
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		Times(4).
		WillRepeatedly(Return(0));

	EXPECT_CALL(relL, Add(&tempL)).
		Times(4).
		WillRepeatedly(Return(true));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(1);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// but only 2 at a time fit in memory,
/// and they are the final records in the pipe
TEST_F(SortMergeJoinTest, StreamLeftGroup6) {
	Sequence s3;
	EXPECT_CALL(inL, Remove(&tempL)).
		Times(4).
		InSequence(s3).
		WillRepeatedly(Return(1));
	EXPECT_CALL(inL, Remove(&tempL)).
		InSequence(s3).
		WillOnce(Return(0));
	
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		Times(4).
		WillRepeatedly(Return(0));

	Sequence s2;
	EXPECT_CALL(relL, Add(&tempL)).
		Times(2).
		InSequence(s2).
		WillRepeatedly(Return(true));
	EXPECT_CALL(relL, Add(&tempL)).
		InSequence(s2).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Add(&tempL)).
		Times(2).
		InSequence(s2).
		WillRepeatedly(Return(true));

	EXPECT_CALL(relL, Clear()).
		Times(1);

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out))).
		Times(2);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, relL, relR, out, memLimit, orderL, comp));
}