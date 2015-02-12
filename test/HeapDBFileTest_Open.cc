#include "HeapDBFileTest.h"

/**
 * DBFile::Open should simply open a file if it exists along with it's header and return 1 if there
 * were no issues. Recall that the indexes will be File.GetLenth() - 2 (unless length is 0)
 */
TEST_F(HeapDBFileTest, Open1) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);	
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s3);
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			InSequence(s2).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(&last, 3)).
			Times(1);
	EXPECT_CALL(mockFile, GetPage(&cursor, 0)).
			Times(1);
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(3, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * If the header fails to open, all files should be closed and Open should return false.
 */
TEST_F(HeapDBFileTest, Open2) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1);

	// s2
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2, s3).
			WillOnce(Return(false));

	EXPECT_CALL(mockFile, Close()).
			InSequence(s3).
			WillOnce(Return(5));

	EXPECT_CALL(config, Clear()). // Arbitrary
			Times(AtMost(2));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * If Read fails, all files should be closed and Open should return 0. Config MUST clear if read
 * fails so that it is not in an unkonwn state.
 */
TEST_F(HeapDBFileTest, Open3) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3, s4;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1);

	// s2
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s4);
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3, s4).
			WillOnce(Return(false));

	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()). // Arbitrary
			InSequence(s4);
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Open should return 0 if the fType indicated by the header is not heap, sorted, or tree.
 * DBConfig MUST clear after this because there are unknown contents in the file.
 */
TEST_F(HeapDBFileTest, Open4) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3, s4, s5;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1);

	// s2
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s4);
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s4).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			InSequence(s2, s3).
			WillOnce(Return(""));
	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()). // Arbitrary
			InSequence(s4);
	// EXPECT_CALL(mockFile, GetLength());
	// EXPECT_CALL(mockFile, GetPage(_,_));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Open should return 0 if no file exists at the provided path. It should also close any
 * files it opened.
 */
TEST_F(HeapDBFileTest, Open5) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3, s4;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1);

	// s2
	EXPECT_CALL(rfile, Open(header)).
			Times(AtMost(1)).
			InSequence(s2).
			WillRepeatedly(Return(true));
	EXPECT_CALL(config, Read(_)).
			Times(AtMost(1)).
			InSequence(s2).
			WillRepeatedly(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			Times(AtMost(1)).
			InSequence(s2, s3).
			WillRepeatedly(Return(""));
	EXPECT_CALL(rfile, Close()).
			Times(AtMost(1)).
			InSequence(s2).
			WillRepeatedly(Return(true));

	EXPECT_CALL(config, Clear()).
			Times(AtMost(2));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Open should return 0 if no header exists at the provided path. It should also close any
 * files it opened.
 */
TEST_F(HeapDBFileTest, Open6) {
	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2;

	// s1
	EXPECT_CALL(mockFile, Open(1, path)). // Arbitrary
		Times(AtMost(1)).
		InSequence(s1);

	EXPECT_CALL(mockFile, Close()).
		Times(AtMost(1)).
		InSequence(s1).
		WillRepeatedly(Return(5));

	EXPECT_CALL(config, Clear()).
		Times(AtMost(1)).
		InSequence(s2);

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Open should return 0 if the path is null.
 */
TEST_F(HeapDBFileTest, Open7) {
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(config, Clear()).
		Times(AtMost(1));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
* DBFile::Open should set cursor to the first page of the file
* and set last to the last page of the file. Recall that the
* indices should be File.GetLength - 2
*/
TEST_F(HeapDBFileTest, Open8) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
		Times(AtMost(1));
	EXPECT_CALL(config, Read(_)).
		WillOnce(Return(true));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(mockFile, Open(1, path)).
			Times(AtMost(1));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));
	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, GetPage(&last, 3));
	

	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(3, LastIndex());

	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
* DBFile::Open should not call GetPage when the 
* file is empty.
*/
TEST_F(HeapDBFileTest, Open9) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);

	SetCursor(cursor);
	SetLast(last);

	
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
		Times(AtMost(1));
	EXPECT_CALL(config, Read(_)).
		WillOnce(Return(true));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(mockFile, Open(1, path)).
			Times(AtMost(1));
	EXPECT_CALL(config, GetKey("fType")).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));
	
	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}
