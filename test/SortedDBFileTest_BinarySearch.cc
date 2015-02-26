#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/RawFile.h"
#include "../include/Comparison.h"
#include "../include/DBConfig.h"

#include "../include/SortedDBFileTest.h"

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 7 (not the start) should set cursorIndex to 7 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch1) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));

	// Last things
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 7)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11, last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));


	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}


/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 3 (not the start) should set cursorIndex to 3 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch2) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 2)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 3)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 3)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 4 (not the start) should set cursorIndex to 5 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch3) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(0));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 4)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 9 (not the start) should set cursorIndex to 9 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch5) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 9)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 9)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 2 (not the start) should set cursorIndex to 2 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch6) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 2)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-2));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 3)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 2)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}


/**
 * SortedDBFile::BinarySearch should, not die if it looks at the cursor but the cursor is empty.
 */
TEST_F(SortedDBFileTest, BinarySearch7) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 2)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 1)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0)).
			InSequence(s1);

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 1)).
			InSequence(s1).
			WillOnce(Return(true));

	Sequence s2;
	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(3).
			InSequence(s2).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s2).
			WillOnce(Return(0));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 5, length 10, and no empty pages
 * with the "correct" Record in Page 7 (not the start) should set cursorIndex to 7 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch8) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 7)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 3, length 10, and no empty pages
 * with the "correct" Record in Page 7 (not the start) should set cursorIndex to 6 and cursor to
 * Page. In this case, the cursorIndex is set to 6 because the first record in record 7 will be a
 * "correct" (ie, == 0).
 */
TEST_F(SortedDBFileTest, BinarySearch9) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(3);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(0));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 6)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 3, length 10, and no empty pages
 * with the "correct" Record in Page 7 (not the start) should set cursorIndex to 6 and cursor to
 * Page. In this case, the cursorIndex is set to 6 because the first record in record 7 will be a
 * "correct" (ie, == 0).
 */
TEST_F(SortedDBFileTest, BinarySearch10) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(3);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 7)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if no match can be found, return false.
 */
TEST_F(SortedDBFileTest, BinarySearch11) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 5)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-8));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 9));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-8));


	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 9)).
			InSequence(s1).
			WillOnce(Return(false));

	EXPECT_EQ(false, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if no match can be found, return false.
 */
TEST_F(SortedDBFileTest, BinarySearch12) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 5)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 3)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 2)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));


	EXPECT_EQ(false, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if a match exists in the first element, return the first
 */
TEST_F(SortedDBFileTest, BinarySearch13) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 5)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 2)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 1)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(0));


	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 0)).
			InSequence(s1).
			WillOnce(Return(true));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::BinarySearch should, if it starts with cursor at 0, length 10, and no empty pages
 * with the "correct" Record in Page 9 (not the start) should set cursorIndex to 9 and cursor to
 * Page.
 */
TEST_F(SortedDBFileTest, BinarySearch14) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1;
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 4)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 6)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8)).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s1).
			WillOnce(Return(-5));

	// Last thing
	EXPECT_CALL(*mock, FindValidRecord(Ref(lit), Ref(query), 8)).
			InSequence(s1).
			WillOnce(Return(true));

	// Arbitrary calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11)); // length of 11,  last index 9
	EXPECT_CALL(page, GetFirst(&rec)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(true, BinarySearch(lit, query, comp, rec, page));

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}
