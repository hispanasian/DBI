#include "HeapDBFileTest.h"

/**
 * DBFile::MoveFirst should ask File for the first (0th) Page and put it into page. It should then
 * update lastIndex to 0. It should also write out the last page
 */
TEST_F(HeapDBFileTest, MoveFirst1) {
	SetCursorIndex(4);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	EXPECT_CALL(mockFile, GetLength()).
				WillRepeatedly(Return(4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}

/**
 * DBFile::MoveFirst should ask File for the first(0th) Page and put it into page even if the
 * current page is already 0. It should also write out the last page and clear out the cursor.
 */
TEST_F(HeapDBFileTest, MoveFirst2) {
	SetCursorIndex(0);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}
