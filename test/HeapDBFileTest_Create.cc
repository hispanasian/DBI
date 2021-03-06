#include "HeapDBFileTest.h"

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's lastIndex unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=heap" in the Config file when heap is
 * passed as the fType. It should also start off by clearing DBConfig. It should also create a
 * delegate of type HeapDBFile.
 */
TEST_F(HeapDBFileTest, CreateHeap) {
	Sequence s1, s2, s3;
	SetLast(last);
	SetCursor(cursor);

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "heap")).
			InSequence(s3);
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(1, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(typeid(HeapDBFile), typeid(*Delegate()));
	SetLastNull();
	SetCursorNull();
}
