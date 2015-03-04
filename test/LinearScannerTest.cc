#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "LinearScanner.h"

using ::testing::StrictMock;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::AtLeast;


/*
 * If cursor still has records, 
 * GetNext should read records from cursor
 * and return 1.
*/
TEST(LinearScannerTest, GetNext1) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* rec;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	EXPECT_CALL(cursor, GetFirst(rec)).
		WillOnce(Return(1));
	EXPECT_CALL(file, GetLength()).
		WillRepeatedly(Return(6));

	ASSERT_EQ(1, ls.GetNext(*rec));
	ASSERT_EQ(0, cursorIndex);
}

/*
 * If cursor is out of records, 
 * GetNext should read the next page from file
 * into cursor, read the next record from cursor,
 * and return 1.
*/
TEST(LinearScannerTest, GetNext2) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(6));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(1));
	EXPECT_CALL(file, GetPage(&cursor, 1));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(AtLeast(1));

	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, cursorIndex);
}

/*
* GetNext() should seek across empty pages to find
* the next non-empty page to get a record from
*/
TEST(LinearScannerTest, GetNext3) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	// the test
	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(8));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(1));
	EXPECT_CALL(file, GetPage(&cursor, 1));
	EXPECT_CALL(file, GetPage(&cursor, 2));
	EXPECT_CALL(file, GetPage(&cursor, 3));
	EXPECT_CALL(file, GetPage(&cursor, 4));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(4);

	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(4, cursorIndex);
}

/*
 * If all of the pages in the file contain
 * 0 records, GetNext() should return 0
 */
TEST(LinearScannerTest, GetNext4) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	// the test
	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(8));
	EXPECT_CALL(cursor, GetFirst(r)).
			Times(7).
			WillRepeatedly(Return(0));
	EXPECT_CALL(file, GetPage(&cursor, 1));
	EXPECT_CALL(file, GetPage(&cursor, 2));
	EXPECT_CALL(file, GetPage(&cursor, 3));
	EXPECT_CALL(file, GetPage(&cursor, 4));
	EXPECT_CALL(file, GetPage(&cursor, 5));
	EXPECT_CALL(file, GetPage(&cursor, 6));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(6);

	ASSERT_EQ(0, ls.GetNext(*r));
	ASSERT_EQ(6, cursorIndex);
}

/*
 * If there are no pages in the file,
 * GetNext() should return zero
 */
TEST(LinearScannerTest, GetNext5) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	// the test
	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(0));

	ASSERT_EQ(0, ls.GetNext(*r));
	ASSERT_EQ(0, cursorIndex);
}



/*
 * After reading all records from the file,
 * GetNext() should continue to return 0 
 * on successive calls
 */

 TEST(LinearScannerTest, GetNext6) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 0;
	off_t& cursorIndex = ci;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex);

	// the test
	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(4));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(0)). // end of page 0
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(0)). // end of page 1
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(0)); // end of page 2
	EXPECT_CALL(file, GetPage(&cursor, 1));
	EXPECT_CALL(file, GetPage(&cursor, 2));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(2);

	
	ASSERT_EQ(0, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(2, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(0, ls.GetNext(*r));
	ASSERT_EQ(2, cursorIndex);
}

/*
 * GetNext() will only read pages in the specified range
 */
TEST(LinearScannerTest, GetNext7) {
	StrictMock<MockFile> file;
	StrictMock<MockPage> cursor;
	Record* r;
	off_t ci = 2;
	off_t& cursorIndex = ci;
	off_t li = 5;
	off_t& lastIndex = li;
	LinearScanner ls = LinearScanner(file, cursor, cursorIndex, lastIndex);

	// the test
	EXPECT_CALL(file, GetLength()).
			WillRepeatedly(Return(10));

	EXPECT_CALL(cursor, GetFirst(r)).
		WillOnce(Return(0)). // end of page 2
		WillOnce(Return(1)).
		WillOnce(Return(0)). // end of page 3
		WillOnce(Return(1)).
		WillOnce(Return(0)). // end of page 4
		WillOnce(Return(1)).
		WillOnce(Return(0)); // end of page 5
	EXPECT_CALL(file, GetPage(&cursor, 3));
	EXPECT_CALL(file, GetPage(&cursor, 4));
	EXPECT_CALL(file, GetPage(&cursor, 5));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(3);

	ASSERT_EQ(2, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(3, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(4, cursorIndex);
	ASSERT_EQ(1, ls.GetNext(*r));
	ASSERT_EQ(5, cursorIndex);
	ASSERT_EQ(0, ls.GetNext(*r));
	ASSERT_EQ(5, cursorIndex);
}