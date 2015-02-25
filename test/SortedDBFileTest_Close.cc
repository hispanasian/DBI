#include "SortedDBFileTest.h"

/**
 * DBFile::Close should call SortedDBFile::Flush
 */
TEST_F(SortedDBFileTest, Close1) {

	MakeFlushlessSorted(sortInfo);
	SetDBTosorteddb();

	Sequence s1, s2, s3;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	EXPECT_CALL(*mock, Flush());
	EXPECT_CALL(mockFile, Close());

	GetOut()->ShutDown();

	EXPECT_EQ(1, file.Close());
	EXPECT_EQ(NULL, Delegate());
}
