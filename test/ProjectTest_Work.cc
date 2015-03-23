#include "ProjectTest.h"
#include "Project.h"

/*
 * Work will project every Record from in and put it into out.
 */

TEST_F(ProjectTest, Work1) {
	int keepMe;

	Sequence s1;
	EXPECT_CALL(in, Remove(&r)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	Sequence s2;
	EXPECT_CALL(r, Project(&keepMe, 5, 10)).
			InSequence(s2);
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(r, Project(&keepMe, 5, 10)).
			InSequence(s2);
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(r, Project(&keepMe, 5, 10)).
			InSequence(s2);
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(r, Project(&keepMe, 5, 10)).
			InSequence(s2);
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);
	EXPECT_CALL(r, Project(&keepMe, 5, 10)).
			InSequence(s2);
	EXPECT_CALL(out, Insert(&r)).
			InSequence(s2);

	// Last Call
	EXPECT_CALL(out, ShutDown()).
			InSequence(s2);

	op.Work(in, out, &keepMe, 10, 5, r);
}

/**
 * Work should not fail if no Record exists in in
 */
TEST_F(ProjectTest, Work2) {
	int keepMe;

	EXPECT_CALL(in, Remove(&r)).
			WillOnce(Return(0));

	// Last Call
	EXPECT_CALL(out, ShutDown());

	op.Work(in, out, &keepMe, 10, 5, r);
}
