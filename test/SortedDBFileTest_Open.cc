#include "SortedDBFileTest.h"

/**
 * DBFile::Open should simply open a file if it exists and create a SortedDBFile with the
 * appropriate values
 */
TEST_F(SortedDBFileTest, Open1) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), 5 };

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s3);
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return("5 int 2 double"));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return("10"));

//	EXPECT_CALL(mockFile, GetPage(_, 3)).
//			Times(1);
//	EXPECT_CALL(mockFile, GetPage(_, 0)).
//			Times(1);
	
	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(typeid(SortedDBFile), typeid(*Delegate()));
	EXPECT_EQ(0, strcmp("5 int 2 double", GetOrder()->ToString().c_str()));
	EXPECT_EQ(10, GetRunLength());
}

/**
 * DBFile::Open should return 0 if no order is found
 */
TEST_F(SortedDBFileTest, Open2) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), 5 };

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1));
	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return(""));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return("10"));

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(NULL, Delegate());
}

/**
 * DBFile::Open should return 0 if no runLength is found
 */
TEST_F(SortedDBFileTest, Open3) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), 5 };

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1));
	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return("5 int 2 double"));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return(""));

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(NULL, Delegate());
}
