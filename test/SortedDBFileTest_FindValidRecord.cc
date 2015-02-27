#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/RawFile.h"
#include "../include/Comparison.h"
#include "../include/DBConfig.h"

#include "../include/SortedDBFileTest.h"

/**
 * SortedDBFile::FindValidRecord should, given an index and a valid record in the page at that index,
 * set the cursor and cursorIndex to that page and return true.
 */
TEST_F(SortedDBFileTest, FindValidRecord1) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(6).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(-5));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(0));

	// Fill up buff with the correct Records
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(buff, Append(&rec)).
			Times(6). // One extra time for the initial Record that we found
			InSequence(s3).
			WillRepeatedly(Return(1));

	// Last things
	EXPECT_CALL(buff, ToBinary(NotNull())).
			InSequence(s1);
	EXPECT_CALL(cursor, FromBinary(NotNull()));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(true, FindValidRecord(lit, query, 0, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should, given an index and a valid record in the page at that index,
 * set the cursor and cursorIndex to that page and return true when that Record is the first
 * Record in the page.
 */
TEST_F(SortedDBFileTest, FindValidRecord2) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(0));

	// Fill up buff with the correct Records
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(buff, Append(&rec)).
			Times(6). // One extra time for the initial Record that we found
			InSequence(s3).
			WillRepeatedly(Return(1));

	// Last things
	EXPECT_CALL(buff, ToBinary(NotNull())).
			InSequence(s3);
	EXPECT_CALL(cursor, FromBinary(NotNull()));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 5));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(true, FindValidRecord(lit, query, 5, rec, page, buff, comp));
	EXPECT_EQ(5, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should check the following Page if the page at index does not
 * contain a conforming Record.
 */
TEST_F(SortedDBFileTest, FindValidRecord3) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(-5));

	// Read from next page
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(-4));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(0));

	// Fill up buff with the correct Records
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(3).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(buff, Append(&rec)).
			Times(4). // One extra time for the initial Record that we found
			InSequence(s3).
			WillRepeatedly(Return(1));

	// Last things
	EXPECT_CALL(buff, ToBinary(NotNull())).
			InSequence(s1);
	EXPECT_CALL(cursor, FromBinary(NotNull()));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 1));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(true, FindValidRecord(lit, query, 0, rec, page, buff, comp));
	EXPECT_EQ(1, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should, given a pages with Records that are all less than the
 * literal/query pair, return false and not change the cursor index.
 */
TEST_F(SortedDBFileTest, FindValidRecord4) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			WillRepeatedly(Return(-5));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 8));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(false, FindValidRecord(lit, query, 7, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should, given a pages with Records that are all greater than the
 * literal/query pair, return false and not change the cursor index.
 */
TEST_F(SortedDBFileTest, FindValidRecord5) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			WillRepeatedly(Return(5));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 7));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(false, FindValidRecord(lit, query, 7, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should, given a pages with Records that are all less than the
 * literal/query pair, return false and not change the cursor index even when the index is the
 * last page
 */
TEST_F(SortedDBFileTest, FindValidRecord6) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			WillRepeatedly(Return(-5));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 9));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(false, FindValidRecord(lit, query, 9, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should check the following Page if the page at index does not
 * contain a conforming Record. If there is a skip from the comparison being less than 0 to
 * greater than 0 (never equal), the second run should catch it. This should return false.
 */
TEST_F(SortedDBFileTest, FindValidRecord7) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(-5));

	// Read from next page
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(-4));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			InSequence(s2).
			WillOnce(Return(5));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 0));
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 1));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(false, FindValidRecord(lit, query, 0, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::FindValidRecord should check the following Page if the page at index does not
 * contain a conforming Record (they are all less than 0). However, if the current page is last,
 * this should just return false.
 */
TEST_F(SortedDBFileTest, FindValidRecord8) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> page;
	StrictMock<MockPage> buff;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	OrderMaker query;
	StrictMock<MockComparisonEngine> comp;

	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(0);

	Sequence s1, s2, s3;
	EXPECT_CALL(page, GetFirst(&rec)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, GetFirst(&rec)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &query)).
			Times(5).
			InSequence(s2).
			WillRepeatedly(Return(-5));

	// Arbitrary calls
	EXPECT_CALL(*mock, GetBSTPage(Ref(page), 9));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10)); // length of 10, last index 9

	EXPECT_EQ(false, FindValidRecord(lit, query, 9, rec, page, buff, comp));
	EXPECT_EQ(0, GetCursorIndex());

	SetIn(NULL);
	SetOut(NULL);
	SetCursor(new Page());
	delete mock;
}
