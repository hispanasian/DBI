#include "SortedDBFileTest.h"
#include "SortedDBFile.h"

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and create a new SorteDBFile with the provided SortOrder.
 */
TEST_F(SortedDBFileTest, CreateSorted1) {
	Sequence s1, s2, s3, s4, s5;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), 5 };

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "sorted")).
			InSequence(s3);
	EXPECT_CALL(config, AddKey("order", "0 int 5 string 6 int")).
			InSequence(s4);
	EXPECT_CALL(config, AddKey("runLength", "5")).
				InSequence(s5);
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s2, s3, s4, s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(1, file.Create(path, sorted, info));
	EXPECT_EQ(typeid(SortedDBFile), typeid(*Delegate()));
	EXPECT_EQ(0, strcmp("0 int 5 string 6 int", GetOrder()->ToString().c_str()));
	EXPECT_EQ(5, GetRunLength());
}

/**
 * DBFile:Create should return 0 if startup is NULL.
 */
TEST_F(SortedDBFileTest, CreateSorted2) {
	Sequence s1, s2, s3, s4, s5;

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, NULL));
	EXPECT_EQ(NULL, Delegate());
}

/**
 * DBFile:Create should return 0 if startup contains a SortInfo struct with a runLength less than
 * or equal to 0.
 */
TEST_F(SortedDBFileTest, CreateSorted3) {
	Sequence s1, s2, s3, s4, s5;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), -1 };

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, info));
	EXPECT_EQ(NULL, Delegate());
}

/**
 * DBFile:Create should return 0 if startup contains a SortInfo struct with a NULL myOrder.
 */
TEST_F(SortedDBFileTest, CreateSorted4) {
	Sequence s1, s2, s3, s4, s5;
	SortInfo *info = new SortInfo{ NULL, 5 };

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, info));
	EXPECT_EQ(NULL, Delegate());
}
