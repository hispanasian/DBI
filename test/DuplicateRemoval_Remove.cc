#include "DuplicateRemovalTest.h"
#include "DuplicateRemoval.h"

/*
 * Remove should put all the Records into out if none match each other.
 */

TEST_F(DuplicateRemovalTest, Remove1) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			Times(5).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	EXPECT_CALL(out, Insert(&r)).
			Times(5);
	
	op.Remove(in, out, schema,r, prev, comp, order);
}

/*
 * Remove should only put one Record into out if they all match
 */
TEST_F(DuplicateRemovalTest, Remove2) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			Times(5).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			Times(4).
			WillRepeatedly(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	EXPECT_CALL(out, Insert(&r)).
			Times(5);

	// "Arbitrary" calls
	EXPECT_CALL(r, Copy(NotNull())).
			Times(AtMost(5));
	EXPECT_CALL(prev, Consume(NotNull())).
			Times(AtMost(5));

	op.Remove(in, out, schema,r, prev, comp, order);
}


/**
 * Remove should be able to handle some matching Records
 */
TEST_F(DuplicateRemovalTest, Remove3) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			Times(5).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(0));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2).
			Times(AtMost(1));
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2).
			Times(AtMost(1));
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(5));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2);
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(NotNull(),NotNull(),&order)).
			InSequence(s2).
			WillOnce(Return(0));
	EXPECT_CALL(r, Copy(NotNull())).
			InSequence(s2).
			Times(AtMost(1));
	EXPECT_CALL(prev, Consume(NotNull())).
			InSequence(s2).
			Times(AtMost(1));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	EXPECT_CALL(out, Insert(&r)).
			Times(3);

	op.Remove(in, out, schema,r, prev, comp, order);
}

/**
 * Select should not fail if no Record exists in in
 */
TEST_F(DuplicateRemovalTest, Remove4) {
	EXPECT_CALL(in, Remove(&r)).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown());

	op.Remove(in, out, schema,r, prev, comp, order);
}

