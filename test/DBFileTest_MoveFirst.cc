#include "DBFileTest.h"

/**
 * DBFile::MoveFirst should ask File for the first (0th) Page and put it into page. It should then
 * update lastIndex to 0. It should also write out the last page
 */
TEST_F(DBFileTest, MoveFirst1) {
	SetCursorIndex(4);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::MoveFirst should ask File for the first(0th) Page and put it into page even if the
 * current page is already 0. It should also write out the last page.
 */
TEST_F(DBFileTest, MoveFirst2) {
	SetCursorIndex(0);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
}
