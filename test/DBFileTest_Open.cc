#include "DBFileTest.h"

/**
 * DBFile::Open should simply open a file if it exists along with it's header and return 1 if there
 * were no issues. Recall that the indexes will be File.GetLenth() - 2 (unless length is 0)
 */
TEST_F(DBFileTest, Open1) {
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
}

/**
 * If the header fails to open, all files should be closed and Open should return false.
 */
TEST_F(DBFileTest, Open2) {
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
}

/**
 * If Read fails, all files should be closed and Open should return 0. Config MUST clear if read
 * fails so that it is not in an unkonwn state.
 */
TEST_F(DBFileTest, Open3) {
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
}

/**
 * DBFile::Open should return 0 if the fType indicated by the header is not heap, sorted, or tree.
 * DBConfig MUST clear after this because there are unknown contents in the file.
 */
TEST_F(DBFileTest, Open4) {
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
}

/**
 * DBFile::Open should return 0 if no file exists at the provided path. It should also close any
 * files it opened.
 */
TEST_F(DBFileTest, Open5) {
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
}

/**
 * DBFile::Open should return 0 if no header exists at the provided path. It should also close any
 * files it opened.
 */
TEST_F(DBFileTest, Open6) {
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
}

/**
 * DBFile::Open should return 0 if the path is null.
 */
TEST_F(DBFileTest, Open7) {
	EXPECT_CALL(config, Clear()).
		Times(AtMost(1));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Open(NULL));
}

/**
* DBFile::Open should set cursor to the first page of the file
* and set last to the last page of the file. Recall that the
* indices should be File.GetLength - 2
*/
TEST_F(DBFileTest, Open8) {
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
}

/**
* DBFile::Open should not call GetPage when the 
* file is empty.
*/
TEST_F(DBFileTest, Open9) {
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
}
