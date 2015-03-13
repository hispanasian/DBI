#include "SelectPipeTest.h"
#include "SelectPipe.h"

/*
 * Select will return the first record if it matches the CNF
 */

TEST_F(SelectPipeTest, Select1) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	EXPECT_CALL(out, Insert(&r)).
			Times(1);

	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			WillRepeatedly(Return(1));
	
	sp.Select(in, out, cnf, literal, comp, r);
}

/*
 * Select should insert any Record that passes the Compare
 */
TEST_F(SelectPipeTest, Select2) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s2);

	sp.Select(in, out, cnf, literal, comp, r);
}


/**
 * Select should not fail if no Records pass
 */
TEST_F(SelectPipeTest, Select3) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s1);

	// No Order
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			WillRepeatedly(Return(0));

	sp.Select(in, out, cnf, literal, comp, r);
}

/**
 * Select should not fail if no Record exists in in
 */
TEST_F(SelectPipeTest, Select4) {
	EXPECT_CALL(in, Remove(&r)).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown());

	sp.Select(in, out, cnf, literal, comp, r);
}


/*
 * Select should not fail if all Records pass
 */
TEST_F(SelectPipeTest, Select5) {
	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(comp, Compare(&r, &literal, &cnf)).
			InSequence(s2).
			WillOnce(Return(1));
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s2);

	sp.Select(in, out, cnf, literal, comp, r);
}

