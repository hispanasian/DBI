#include "HeapDBFileTest.h"

/*
 * GetNext() should call GetFirst() on the cursor
 * and pass addMe as a paramter
 */
TEST_F(HeapDBFileTest, GetNext1) {
	// setup
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(6));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1));

	
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(0, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * If cursor has no more records, GetNext() should increment
 * the cursor index, read the next page into cursor,
 * and return the next record
 */
TEST_F(HeapDBFileTest, GetNext2) {
	// setup
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(6));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(AtLeast(1));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}


/*
* GetNext() should seek across empty pages to find
* the next non-empty page to get a record from
*/
TEST_F(HeapDBFileTest, GetNext3) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(8));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 2));
	EXPECT_CALL(mockFile, GetPage(&cursor, 3));
	EXPECT_CALL(mockFile, GetPage(&cursor, 4));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(4);

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(4, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * If all of the pages in the file contain
 * 0 records, GetNext() should return 0
 */
TEST_F(HeapDBFileTest, GetNext4) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(8));
	EXPECT_CALL(cursor, GetFirst(r)).
			Times(7).
			WillRepeatedly(Return(0));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 2));
	EXPECT_CALL(mockFile, GetPage(&cursor, 3));
	EXPECT_CALL(mockFile, GetPage(&cursor, 4));
	EXPECT_CALL(mockFile, GetPage(&cursor, 5));
	EXPECT_CALL(mockFile, GetPage(&cursor, 6));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(6);

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, file.GetNext(*r));
	EXPECT_EQ(6, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * If there are no pages in the file,
 * GetNext() should return zero
 */
TEST_F(HeapDBFileTest, GetNext5) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, file.GetNext(*r));
	EXPECT_EQ(0, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}



/*
 * After reading all records from the file,
 * GetNext() should continue to return 0 
 * on successive calls
 */

 TEST_F(HeapDBFileTest, GetNext6) {
	// setup
	 SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
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
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 2));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(2);

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, file.GetNext(*r)); // 1st record from page 1
	EXPECT_EQ(1, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(2, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r)); // 1st record from page 2
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(0, file.GetNext(*r));
	EXPECT_EQ(2, CursorIndex());


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * GetNext will return the first record if it matches the CNF
 */

TEST_F(HeapDBFileTest, GetNextCNF1) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(4));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillRepeatedly(Return(1));
	
	EXPECT_EQ(1, file.GetNext(*r, *cnf, *literal));


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * GetNext will read through multiple records until it finds 
 * one that matches 
 */
TEST_F(HeapDBFileTest, GetNextCNF2) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(4));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(1));

	EXPECT_EQ(1, file.GetNext(*r, *cnf, *literal));


	// cleanup
	SetCursorNull();
	SetLastNull();
}


/*
 * If none of the records match, GetNext will return 0
 */

TEST_F(HeapDBFileTest, GetNextCNF3) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(3));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(0)).
			WillOnce(Return(1)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillRepeatedly(Return(0));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(1);

	EXPECT_EQ(0, file.GetNext(*r, *cnf, *literal));


	// cleanup
	SetCursorNull();
	SetLastNull();
}

/*
 * Files with 0 pages are properly handled
 */
TEST_F(HeapDBFileTest, GetNextCNF4) {
	// setup
	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));
	EXPECT_EQ(0, file.GetNext(*r, *cnf, *literal));


	// cleanup
	SetCursorNull();
	SetLastNull();
}
