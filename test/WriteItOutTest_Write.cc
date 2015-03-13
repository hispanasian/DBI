#include "WriteItOutTest.h"

/*
 * Write will return the first record if it matches the CNF
 */

TEST_F(WriteItOutTest, Write1) {
	Sequence s1;

	// First Call
	EXPECT_CALL(file, Open(f)).
			InSequence(s1).
			WillOnce(Return(true));
	
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(file, Append("")).
			InSequence(s1);
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(file, Append("")).
			InSequence(s1);
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(file, Append("")).
			InSequence(s1);
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(file, Append("")).
			InSequence(s1);
	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	EXPECT_CALL(r, ToString(&schema)).
			WillRepeatedly(Return(""));

	op.Write(in, f, schema, file, r);
}


/*
 * Write should not fail if there are no Records
 */

TEST_F(WriteItOutTest, Write2) {
	Sequence s1;

	// First Call
	EXPECT_CALL(file, Open(f)).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_CALL(in, Remove(&r)).
			InSequence(s1).
			WillOnce(Return(0));

	op.Write(in, f, schema, file, r);
}
