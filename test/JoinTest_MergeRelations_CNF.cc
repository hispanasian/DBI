#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/JoinTest.h"
#include "Defs.h"


/**
 * JoinRelations should handle multiple passing and failing Records (according to the CNF).
 * This specific test will test R with 3 Records and S with  2 Records
 */
TEST_F(JoinTest, JoinRelations_CNF1) {
	StrictMock<MockRecord> recR;
	StrictMock<MockRecord> recS;

	// First, compute the merge-related data
	Sequence s1, s2;
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));

	EXPECT_CALL(recR, NumAtts()).
			InSequence(s1).
			WillOnce(Return(3));
	EXPECT_CALL(recS, NumAtts()).
			InSequence(s2).
			WillOnce(Return(5));

	// Perform initial join
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);

	// Now join Records. Outer loop on relS and inner join on relR

	// First loop on S
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset R
	EXPECT_CALL(relR, Reset()).
			InSequence(s1, s2);

	// Second loop on R
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset R
	EXPECT_CALL(relR, Reset()).
			Times(AtMost(1)).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			Times(AtMost(1)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));

	// No more Records
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	MergeRelations(relR, relS, rec, out, comp, literal, cnf);
}

/**
 * JoinRelations should not fail if none of the Records pass the comparison
 */
TEST_F(JoinTest, JoinRelations_CNF2) {
	StrictMock<MockRecord> recR;
	StrictMock<MockRecord> recS;

	// First, compute the merge-related data
	Sequence s1, s2;
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));

	EXPECT_CALL(recR, NumAtts()).
			InSequence(s1).
			WillOnce(Return(3));
	EXPECT_CALL(recS, NumAtts()).
			InSequence(s2).
			WillOnce(Return(5));

	// Now join Records. Outer loop on relS and inner loop on relR

	// First loop on S
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset R
	EXPECT_CALL(relR, Reset()).
			InSequence(s1, s2);

	// Second loop on S
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset R
	EXPECT_CALL(relR, Reset()).
			InSequence(s1, s2).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			Times(AtMost(1)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));

	// No more Records
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(false)));

	// Arbitrary Calls
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			WillRepeatedly(Return(false));

	MergeRelations(relR, relS, rec, out, comp, literal, cnf);
}


/**
 * JoinRelations should not fail if all of the Records pass the comparison
 */
TEST_F(JoinTest, JoinRelations_CNF3) {
	StrictMock<MockRecord> recR;
	StrictMock<MockRecord> recS;

	// First, compute the merge-related data
	Sequence s1, s2;
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));

	EXPECT_CALL(recR, NumAtts()).
			InSequence(s1).
			WillOnce(Return(3));
	EXPECT_CALL(recS, NumAtts()).
			InSequence(s2).
			WillOnce(Return(5));

	// Perform initial join
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);

	// Now join Records. Outer loop on relS and inner join on relR

	// First loop on S
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset S
	EXPECT_CALL(relR, Reset()).
			InSequence(s1, s2);

	// Second loop on R
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(true)));
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));
	EXPECT_CALL(rec, MergeRecords(&recR, &recS, 3, 5, NotNull(), 8, 3)).
			InSequence(s1, s2);
	EXPECT_CALL(out, Insert(NotNull())).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			InSequence(s1, s2).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(false)));

	// Now Reset R
	EXPECT_CALL(relR, Reset()).
			Times(AtMost(1)).
			InSequence(s1, s2);
	EXPECT_CALL(relR, GetNext(_)).
			Times(AtMost(1)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&recR), Return(true)));

	// No more Records
	EXPECT_CALL(relS, GetNext(_)).
			InSequence(s2).
			WillOnce(DoAll(SetArgReferee<0>(&recS), Return(false)));

	// Arbitrary Calls
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &literal, &cnf)).
			WillRepeatedly(Return(true));

	MergeRelations(relR, relS, rec, out, comp, literal, cnf);
}
