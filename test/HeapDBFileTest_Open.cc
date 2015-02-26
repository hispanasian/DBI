#include "HeapDBFileTest.h"

/**
 * DBFile::Open should simply open a file if it exists along with it's header and return 1 if there
 * were no issues. Recall that the indexes will be File.GetLenth() - 2 (unless length is 0)
 */
TEST_F(HeapDBFileTest, Open1) {
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

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
			InSequence(s2).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(_, 3)).
			Times(1);
	EXPECT_CALL(mockFile, GetPage(_, 0)).
			Times(1);
	
	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, GetDelegateCursorIndex());
	EXPECT_EQ(3, GetDelegateLastIndex());
	EXPECT_EQ(typeid(HeapDBFile), typeid(*Delegate()));

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}
