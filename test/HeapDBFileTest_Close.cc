#include "HeapDBFileTest.h"

/**
 * DBFile::Close should call File.Close, write last to disk, and clear out last, cursor, lastIndex,
 * and cursorIndex. It should then return 1.
 */
TEST_F(HeapDBFileTest, Close1) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(6);

	MockPage *cursor = new StrictMock<MockPage>;
	MockPage *last = new StrictMock<MockPage>;
	SetCursorNull();
	SetLastNull();
	SetCursorP(cursor);
	SetLastP(last);
	MockPage &lastRef = *last;

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&lastRef, 6)).
			InSequence(s3);
	EXPECT_CALL(*last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(*cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());


	EXPECT_EQ(1, file.Close());
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(NULL, Delegate());

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}

/**
 * DBFile::Close should return false if DBConfig::Write fails, but it should still close RawFile,
 * clear config, zero out the indexes, and write out last.
 */
TEST_F(HeapDBFileTest, Close2) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(6);

	MockPage *cursor = new StrictMock<MockPage>;
	MockPage *last = new StrictMock<MockPage>;
	SetCursorNull();
	SetLastNull();
	SetCursorP(cursor);
	SetLastP(last);
	MockPage &lastRef = *last;

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&lastRef, 6)).
			InSequence(s3);
	EXPECT_CALL(*last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(*cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(0, file.Close());
	// EXPECT_EQ(0, CursorIndex());
	// EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(NULL, Delegate());

	// SetCursorNull();
	// SetLastNull();
	SetDBNull();
}

/**
 * DBFile::Close should add page to File if a record has been added to the current page. However,
 * it should also return false if RawFile::Close fails. It should also still write out the last
 * page and zero out the indexes.
 */
TEST_F(HeapDBFileTest, Close4) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(6);

	MockPage *cursor = new StrictMock<MockPage>;
	MockPage *last = new StrictMock<MockPage>;
	SetCursorNull();
	SetLastNull();
	SetCursorP(cursor);
	SetLastP(last);
	MockPage &lastRef = *last;

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(false));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&lastRef, 6)).
			InSequence(s3);
	EXPECT_CALL(*last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(*cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(0, file.Close());
	// EXPECT_EQ(0, CursorIndex());
	// EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(NULL, Delegate());

	// SetCursorNull();
	// SetLastNull();
	SetDBNull();
}

/**
 * DBFile::Close should write last to disk, even if last is the first page (or there are no other
 * pages).
 */
TEST_F(HeapDBFileTest, Close5) {
	SetDBToheapdb();
	SetCursorIndex(0);
	SetLastIndex(0);

	MockPage *cursor = new StrictMock<MockPage>;
	MockPage *last = new StrictMock<MockPage>;
	SetCursorNull();
	SetLastNull();
	SetCursorP(cursor);
	SetLastP(last);
	MockPage &lastRef = *last;

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&lastRef, 0)).
			InSequence(s3);
	EXPECT_CALL(*last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(*cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(1, file.Close());
	// EXPECT_EQ(0, CursorIndex());
	// EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(NULL, Delegate());

	// SetCursorNull();
	// SetLastNull();
	SetDBNull();
}
