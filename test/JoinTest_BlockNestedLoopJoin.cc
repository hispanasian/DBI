#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/JoinTest.h"
#include "Defs.h"

/**
 * BlockNestedLoopJoin should not fail if inR is empty
 */
TEST_F(JoinTest, BlockNestedLoopJoin1) {
	InSequence seq;
	EXPECT_CALL(jrel, Populate(Ref(inR))).
			WillOnce(Return(true));
	EXPECT_CALL(jrel, MemUsed()).
			WillOnce(Return(0));

	// Last call
	EXPECT_CALL(out, ShutDown());

	EXPECT_CALL(imrel, Clear()).
			Times(AtMost(1));

	BlockNestedLoopJoin(inL, inR, out, cnf, literal, jrel, imrel, rec);
}

/**
 * BlockNestedLoopJoin should not fail if inL is empty
 */
TEST_F(JoinTest, BlockNestedLoopJoin2) {
	SetPageLimit(1);
	SetMemLimit(PAGE_SIZE);

	Sequence s1;
	EXPECT_CALL(jrel, Populate(Ref(inR))).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_CALL(inL, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(false));
	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));

	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	EXPECT_CALL(imrel, Clear()).
			Times(AtMost(1));

	// Arbitrary Calls
	EXPECT_CALL(jrel, MemUsed()).
			WillRepeatedly(Return(150));
	EXPECT_CALL(imrel, MemUsed()).
			Times(AtMost(1)).
			WillRepeatedly(Return(0));

	BlockNestedLoopJoin(inL, inR, out, cnf, literal, jrel, imrel, rec);
}

/**
 * BlockNestedLoopJoin should join inL with inR and should not fail if inL does not consume all of
 * the memory alotted to imrel
 */
TEST_F(JoinTest, BlockNestedLoopJoin3) {
	SetPageLimit(1);
	SetMemLimit(PAGE_SIZE);

	Sequence s1;
	EXPECT_CALL(jrel, Populate(Ref(inR))).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));


	// A total of 2 elements
	Sequence s3;
	EXPECT_CALL(inL, Remove(NotNull())).
			Times(2).
			InSequence(s3).
			WillRepeatedly(Return(true));
	EXPECT_CALL(inL, Remove(NotNull())).
			InSequence(s3).
			WillOnce(Return(false));

	// Now to join the elements.
	EXPECT_CALL(imrel, MemUsed()).
			InSequence(s3).
			WillRepeatedly(Return(3));
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s3);

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s3);

	// it'll happen some time...
	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));
	EXPECT_CALL(imrel, Clear()).
			Times(AtMost(1));

	// Arbitrary Call
	EXPECT_CALL(imrel, Add(NotNull())).
			WillRepeatedly(Return(true));
	EXPECT_CALL(jrel, MemUsed()).
			WillRepeatedly(Return(150));

	BlockNestedLoopJoin(inL, inR, out, cnf, literal,jrel, imrel, rec);
}

/**
 * BlockNestedLoopJoin should join inL with inR and add 2 "groups" of Records to imrel, join them
 * with jrel, and then put the final third group in imrel (which will not consume it completely),
 * and join that with jrel
 */
TEST_F(JoinTest, BlockNestedLoopJoin4) {
	SetPageLimit(1);
	SetMemLimit(PAGE_SIZE);

	Sequence s1;
	EXPECT_CALL(jrel, Populate(Ref(inR))).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));


	// A total of 2 elements will be added in Group 1
	Sequence s2, s3;
	EXPECT_CALL(imrel, Add(NotNull())).
			Times(2).
			InSequence(s2).
			WillRepeatedly(Return(true));
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2).
			WillOnce(Return(false));

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(3).
			InSequence(s3).
			WillRepeatedly(Return(true));

	// Now to join the elements.
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s2, s3);

	// Clear imrel
	EXPECT_CALL(imrel, Clear()).
			InSequence(s2, s3);
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2, s3).
			WillOnce(Return(true));

	// A total of 6 elements will be added in Group 2 (one from the last group)
	EXPECT_CALL(imrel, Add(NotNull())).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(true));
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2).
			WillOnce(Return(false));

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(6).
			InSequence(s3).
			WillRepeatedly(Return(true));

	// Now to join the elements.
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s2, s3);

	// Clear imrel
	EXPECT_CALL(imrel, Clear()).
			InSequence(s2, s3);
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2, s3).
			WillOnce(Return(true));

	// A total of 4 elements will be added in the last group (terminated by the pipe). (4 including
	// Record from last group that could not fit
	EXPECT_CALL(imrel, Add(NotNull())).
			Times(3).
			InSequence(s2).
			WillRepeatedly(Return(true));

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(3).
			InSequence(s3).
			WillRepeatedly(Return(true));
	EXPECT_CALL(inL, Remove(NotNull())).
			InSequence(s3).
			WillOnce(Return(false));

	// Now to join the elements.
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s2, s3);

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(AtMost(1)).
			InSequence(s2, s3).
			WillRepeatedly(Return(false));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s2);

	// it'll happen some time...
	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));
	EXPECT_CALL(jrel, MemUsed()).
			WillRepeatedly(Return(150));
	EXPECT_CALL(imrel, MemUsed()).
			Times(AtMost(1)).
			WillRepeatedly(Return(5));

	BlockNestedLoopJoin(inL, inR, out, cnf, literal,jrel, imrel, rec);
}

/**
 * BlockNestedLoopJoin should join inL with inR and add 2 "groups" of Records to imrel, join them
 * with jrel, and not fail when no Records remain (ie, both groups filled imrel).
 */
TEST_F(JoinTest, BlockNestedLoopJoin5) {
	SetPageLimit(1);
	SetMemLimit(PAGE_SIZE);

	Sequence s1;
	EXPECT_CALL(jrel, Populate(Ref(inR))).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));


	// A total of 2 elements will be added in Group 1
	Sequence s2, s3;
	EXPECT_CALL(imrel, Add(NotNull())).
			Times(2).
			InSequence(s2).
			WillRepeatedly(Return(true));
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2).
			WillOnce(Return(false));

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(3).
			InSequence(s3).
			WillRepeatedly(Return(true));

	// Now to join the elements.
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s2, s3);

	// Clear imrel
	EXPECT_CALL(imrel, Clear()).
			InSequence(s2, s3);
	EXPECT_CALL(imrel, Add(NotNull())).
			InSequence(s2, s3).
			WillOnce(Return(true));

	// A total of 6 elements will be added in Group 2 (one from last group)
	EXPECT_CALL(imrel, Add(NotNull())).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(true));

	EXPECT_CALL(inL, Remove(NotNull())).
			Times(5).
			InSequence(s3).
			WillRepeatedly(Return(true));
	EXPECT_CALL(inL, Remove(NotNull())).
			InSequence(s3).
			WillOnce(Return(false));

	// Now to join the elements.
	EXPECT_CALL(op, MergeRelations(Ref(imrel), Ref(jrel), Ref(rec), Ref(out), _, Ref(literal), Ref(cnf))).
			InSequence(s2, s3);

	// Clear imrel
	EXPECT_CALL(imrel, Clear()).
			Times(AtMost(1)).
			InSequence(s2, s3);

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s2);

	// it'll happen some time...
	EXPECT_CALL(imrel, SetMemLimit(PAGE_SIZE-150)).
			Times(AtMost(1));
	EXPECT_CALL(imrel, MemUsed()).
			Times(AtLeast(1)).
			WillRepeatedly(Return(1));

	EXPECT_CALL(jrel, MemUsed()).
			WillRepeatedly(Return(150));

	BlockNestedLoopJoin(inL, inR, out, cnf, literal,jrel, imrel, rec);
}
