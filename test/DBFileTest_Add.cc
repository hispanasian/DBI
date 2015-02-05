#include "DBFileTest.h"

/**
 * DBFile::Add should, if last is not full, add the record to last.
 */
TEST_F(DBFileTest, Add1) {
	SetCursorIndex(0);
	SetLastIndex(5);

	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	Sequence s1;
	EXPECT_CALL(last, Append(&record)).
			InSequence(s1).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is not full and cursor is on the last page, add the record to last.
 */
TEST_F(DBFileTest, Add2) {
	SetCursorIndex(5);
	SetLastIndex(5);

	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	Sequence s1;
	EXPECT_CALL(last, Append(&record)).
			InSequence(s1).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is full, write out the last page and add the record to the cleared
 * last.
 */
TEST_F(DBFileTest, Add3) {
	SetCursorIndex(0);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, AddPage(&last, 5));
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(6, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, even if the last page is the current page, write the record to last.
 */
TEST_F(DBFileTest, Add4) {
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, even if the last page is the first page AND the current page, write the
 * record to last.
 */
TEST_F(DBFileTest, Add5) {
	SetCursorIndex(0);
	SetLastIndex(0);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is full and cursor is at the end, write out the last page and add
 * the record to the cleared last.
 */
TEST_F(DBFileTest, Add6) {
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, AddPage(&last, 5));
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(6, LastIndex());

	SetCursorNull();
	SetLastNull();
}
