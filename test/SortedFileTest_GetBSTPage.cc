#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/RawFile.h"
#include "../include/Comparison.h"
#include "../include/DBConfig.h"

#include "../include/SortedDBFileTest.h"

/**
 * SortedDBFile::GetBSTPage should return a copy of cursor if the index matches
 * cursorIndex
 */
TEST_F(SortedDBFileTest, GetBSTPage1) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> temp;

	SetCursor(&cursor);
	SetCursorIndex(2);

	InSequence seq;
	EXPECT_CALL(cursor, ToBinary(_));
	EXPECT_CALL(temp, FromBinary(NotNull()));

	GetBSTPage(temp, 2);
	SetIn(NULL);
	SetOut(NULL);
	SetCursor(NULL);
	delete mock;
}

/**
 * SortedDBFile::GetBSTPage should get a copy of the page from file if the index does
 * not match cursorIndex
 */
TEST_F(SortedDBFileTest, GetBSTPage2) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> temp;

	SetCursor(&cursor);
	SetCursorIndex(2);

	EXPECT_CALL(mockFile, GetPage(&temp, 6));

	GetBSTPage(temp, 6);
	SetIn(NULL);
	SetOut(NULL);
	SetCursor(NULL);
	delete mock;
}
