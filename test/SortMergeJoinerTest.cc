#include "SortMergeJoinerTest.h"


// All records on left are > those on right
TEST_F(SortMergeJoinerTest, AlignGroups1) {
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
TEST_F(SortMergeJoinerTest, AlignGroups2) {
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
TEST_F(SortMergeJoinerTest, AlignGroups3) {
	EXPECT_CALL(comp, Compare(&tempL, &orderL, &tempR, &orderR)).
		WillRepeatedly(Return(0));

	EXPECT_EQ(false, AlignGroups(inL, inR, tempL, tempR, orderR, orderL, comp));
}

// A match is found after advancing the left
TEST_F(SortMergeJoinerTest, AlignGroups4) {
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
TEST_F(SortMergeJoinerTest, AlignGroups5) {
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
TEST_F(SortMergeJoinerTest, InitRightGroup1) {
	EXPECT_CALL(inR, Remove(&tempR)).
		WillOnce(Return(0));

	EXPECT_EQ(true, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next record in the pipe is not part of this group
TEST_F(SortMergeJoinerTest, InitRightGroup2) {
	EXPECT_CALL(inR, Remove(&tempR)).
		WillOnce(Return(1));

	EXPECT_CALL(comp, Compare(&groupRecR, &tempR, &orderR)).
		WillOnce(Return(1));

	EXPECT_EQ(false, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next 3 records in the pipe are part of the group,
// but the 4th is not in the group
TEST_F(SortMergeJoinerTest, InitRightGroup3) {
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

	EXPECT_CALL(relR, Add(&tempR)).
		Times(3).
		WillRepeatedly(Return(true));

	EXPECT_EQ(false, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The next 3 records in the pipe are part of the group,
// and they are the final records in the pipe
TEST_F(SortMergeJoinerTest, InitRightGroup4) {
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

	EXPECT_CALL(relR, Add(&tempR)).
		Times(3).
		WillRepeatedly(Return(true));

	EXPECT_EQ(true, InitRightGroup(inR, groupRecR, tempR, relR, orderR, comp));
}

// The left pipe is empty
TEST_F(SortMergeJoinerTest, StreamLeftGroup1) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		WillOnce(Return(0));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(1);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// The next record in the pipe is not part of this group
TEST_F(SortMergeJoinerTest, StreamLeftGroup2) {
	EXPECT_CALL(relR, MemUsed()).
		WillOnce(Return(500));
	EXPECT_CALL(relL, SetMemLimit(500)).
		Times(1);

	EXPECT_CALL(inL, Remove(&tempL)).
		WillOnce(Return(1));

	EXPECT_CALL(comp, Compare(&groupRecL, &tempL, &orderL)).
		WillOnce(Return(1));

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(1);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// and they all fit in memory
// but the 5th is not in the group
TEST_F(SortMergeJoinerTest, StreamLeftGroup3) {
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

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(1);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// but only 2 at a time fit in memory,
// and the 5th is not in the group
TEST_F(SortMergeJoinerTest, StreamLeftGroup4) {
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

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(2);

	EXPECT_EQ(false, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// and they all fit in memory
// and they are the final records in the pipe
TEST_F(SortMergeJoinerTest, StreamLeftGroup5) {
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

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(1);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// The next 4 records in the pipe are part of the group,
// but only 2 at a time fit in memory,
/// and they are the final records in the pipe
TEST_F(SortMergeJoinerTest, StreamLeftGroup6) {
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

	EXPECT_CALL(mock, MergeRelations(Ref(relL), Ref(relR), Ref(out), Ref(mergeInto))).
		Times(2);

	EXPECT_EQ(true, StreamLeftGroup(inL, groupRecL, tempL, mergeInto, relL, relR, out, memLimit, orderL, comp));
}

// Both the left and right have 1 record each
TEST_F(SortMergeJoinerTest, MergeRelations1) {
	StrictMock<MockRecord> recL;
	StrictMock<MockRecord> recR;

	Sequence s1;
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillRepeatedly(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));

	Sequence s2;
	EXPECT_CALL(relR, Reset()).
		Times(1).
		InSequence(s2);
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillRepeatedly(Return(false));

	EXPECT_CALL(recL, NumAtts()).
		WillOnce(Return(2));
	EXPECT_CALL(recR, NumAtts()).
		WillOnce(Return(3));

	EXPECT_CALL(mergeInto, MergeRecords(NotNull(), NotNull(), 2, 3, NotNull(), 5, 2)).
		Times(1);

	EXPECT_CALL(out, Insert(&mergeInto)).
		Times(1);

	MergeRelations(relL, relR, out, mergeInto);
}

// The left has 3 records and the right has 1 record
TEST_F(SortMergeJoinerTest, MergeRelations2) {
	StrictMock<MockRecord> recL;
	StrictMock<MockRecord> recR;

	Sequence s1;
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		Times(3).
		InSequence(s1).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));

	Sequence s2;
	EXPECT_CALL(relR, Reset()).
		Times(1).
		InSequence(s2);
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillRepeatedly(Return(false));

	EXPECT_CALL(recL, NumAtts()).
		WillOnce(Return(2));
	EXPECT_CALL(recR, NumAtts()).
		WillOnce(Return(3));

	EXPECT_CALL(mergeInto, MergeRecords(NotNull(), NotNull(), 2, 3, NotNull(), 5, 2)).
		Times(3);

	EXPECT_CALL(out, Insert(&mergeInto)).
		Times(3);

	MergeRelations(relL, relR, out, mergeInto);
}

// The left has 1 record and the right has 3 record
TEST_F(SortMergeJoinerTest, MergeRelations3) {
	StrictMock<MockRecord> recL;
	StrictMock<MockRecord> recR;

	Sequence s1;
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));

	Sequence s2;
	EXPECT_CALL(relR, Reset()).
		Times(1).
		InSequence(s2);
	EXPECT_CALL(relR, GetNext(NotNull())).
		Times(3).
		InSequence(s2).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillRepeatedly(Return(false));

	EXPECT_CALL(recL, NumAtts()).
		WillOnce(Return(2));
	EXPECT_CALL(recR, NumAtts()).
		WillOnce(Return(3));

	EXPECT_CALL(mergeInto, MergeRecords(NotNull(), NotNull(), 2, 3, NotNull(), 5, 2)).
		Times(3);

	EXPECT_CALL(out, Insert(&mergeInto)).
		Times(3);

	MergeRelations(relL, relR, out, mergeInto);
}

// The left and right have 3 records each
TEST_F(SortMergeJoinerTest, MergeRelations4) {
	StrictMock<MockRecord> recL;
	StrictMock<MockRecord> recR;

	Sequence s1;
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		Times(3).
		InSequence(s1).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		Times(3).
		InSequence(s1).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		Times(3).
		InSequence(s1).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recL), Return(true)));
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(Return(false));
	EXPECT_CALL(relL, Reset()).
		Times(1).
		InSequence(s1);
	EXPECT_CALL(relL, GetNext(NotNull())).
		InSequence(s1).
		WillOnce(DoAll(SetArgReferee<0>(&recL), Return(true)));

	Sequence s2;
	EXPECT_CALL(relR, Reset()).
		Times(1).
		InSequence(s2);
	EXPECT_CALL(relR, GetNext(NotNull())).
		Times(3).
		InSequence(s2).
		WillRepeatedly(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(NotNull())).
		InSequence(s2).
		WillRepeatedly(Return(false));

	EXPECT_CALL(recL, NumAtts()).
		WillOnce(Return(2));
	EXPECT_CALL(recR, NumAtts()).
		WillOnce(Return(3));

	EXPECT_CALL(mergeInto, MergeRecords(NotNull(), NotNull(), 2, 3, NotNull(), 5, 2)).
		Times(9);

	EXPECT_CALL(out, Insert(&mergeInto)).
		Times(9);

	MergeRelations(relL, relR, out, mergeInto);
}
