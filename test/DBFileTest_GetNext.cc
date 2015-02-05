#include "DBFileTest.h"

void createFiles(char* header, char* path) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
}

void cleanupFiles(char* header, char* path) {
	remove(path);
	remove(header);
}

/*
 * GetNext() should call GetFirst() on the cursor
 * and pass addMe as a paramter
 */
TEST_F(DBFileTest, GetNext1) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 4));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1));

	EXPECT_EQ(1, file.Open(path));

	
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(0, CursorIndex());


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * If cursor has no more records, GetNext() should increment
 * the cursor index, read the next page into cursor,
 * and return the next record
 */
TEST_F(DBFileTest, GetNext2) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 4));
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(2);
	
	EXPECT_EQ(1, file.Open(path));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(1, CursorIndex());


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}


/*
* GetNext() should seek across empty pages to find
* the next non-empty page to get a record from
*/
TEST_F(DBFileTest, GetNext3) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 6));
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(7));
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
			Times(5);
	
	EXPECT_EQ(1, file.Open(path));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(4, CursorIndex());


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * If all of the pages in the file contain
 * 0 records, GetNext() should return 0
 */
TEST_F(DBFileTest, GetNext4) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 6));
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(7));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(mockFile, GetPage(&cursor, 2));
	EXPECT_CALL(mockFile, GetPage(&cursor, 3));
	EXPECT_CALL(mockFile, GetPage(&cursor, 4));
	EXPECT_CALL(mockFile, GetPage(&cursor, 5));
	EXPECT_CALL(mockFile, GetPage(&cursor, 6));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(7);
	
	EXPECT_EQ(1, file.Open(path));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, file.GetNext(*r));
	EXPECT_EQ(6, CursorIndex());


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * If there are no pages in the file,
 * GetNext() should return zero
 */
TEST_F(DBFileTest, GetNext5) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));
	
	EXPECT_EQ(1, file.Open(path));

	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, file.GetNext(*r));
	EXPECT_EQ(0, CursorIndex());


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}



/*
 * After reading all records from the file,
 * GetNext() should continue to return 0 
 * on successive calls
 */

 TEST_F(DBFileTest, GetNext6) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 2));
	EXPECT_CALL(last, EmptyItOut());


	Record* r;

	// the test
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(3));
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
			Times(3);
	
	EXPECT_EQ(1, file.Open(path));

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
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * GetNext will return the first record if it matches the CNF
 */

TEST_F(DBFileTest, GetNextCNF1) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 4));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(cursor, GetFirst(r)).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillRepeatedly(Return(1));

	EXPECT_EQ(1, file.Open(path));

	
	EXPECT_EQ(1, file.GetNext(*r, *cnf, *literal));


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * GetNext will read through multiple records until it finds 
 * one that matches 
 */
TEST_F(DBFileTest, GetNextCNF2) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 4));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(cursor, GetFirst(r)).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(0)).
			WillOnce(Return(1));

	EXPECT_EQ(1, file.Open(path));

	
	EXPECT_EQ(1, file.GetNext(*r, *cnf, *literal));


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}


/*
 * If none of the records match, GetNext will return 0
 */

TEST_F(DBFileTest, GetNextCNF3) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(2));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&cursor, 1));
	EXPECT_CALL(mockFile, GetPage(&last, 1));
	EXPECT_CALL(cursor, EmptyItOut()).
			Times(2);
	EXPECT_CALL(last, EmptyItOut());


	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(1)).
			WillOnce(Return(0)).
			WillOnce(Return(1)).
			WillOnce(Return(0));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillRepeatedly(Return(0));

	EXPECT_EQ(1, file.Open(path));

	
	EXPECT_EQ(0, file.GetNext(*r, *cnf, *literal));


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}

/*
 * Files with 0 pages are properly handled
 */
TEST_F(DBFileTest, GetNextCNF4) {
	// setup
	createFiles(header, path);
	SetCursor(cursor);
	SetLast(last);

	// standard stuff for calling Open()
	EXPECT_CALL(mockFile, Open(1, path));
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
			EXPECT_CALL(config, Clear());
	EXPECT_CALL(config, Read(_)).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	Record* r;
	Record* literal;
	CNF* cnf;

	// the test
	EXPECT_EQ(1, file.Open(path));

	
	EXPECT_EQ(0, file.GetNext(*r, *cnf, *literal));


	// cleanup
	cleanupFiles(header, path);
	SetCursorNull();
	SetLastNull();
}
