//#include "SortedDBFileTest.h"
//
////class PartialSortedDBFileMock: SortedDBFile {
////
////};
//
///**
// * SortedDBFile::Flush should create a temporary File, use it to put in the new Records, and then
// * rename it to the current file in the SortedRecord. Finally, SortedDBFile should close the temp
// * and current File, delete the "old" file and rename the new file.
// */
//TEST_F(SortedDBFileTest, Flush1) {
//	StrictMock<MockPipe> in;
//	StrictMock<MockPipe> out;
//	StrictMock<MockFile> temp;
//	StrictMock<MockBigQ> q;
//	char *tempname = "tempXXXXXX";
//	char *oldname = "asdasdasd.bin";
//
//	SetIn(&in);
//	SetOut(&out);
//	SetBigQ(&q);
//
//	Sequence s1, s2;
//	EXPECT_CALL(rfile, MakeTemp(NotNull())).
//			InSequence(s1, s2);
//	EXPECT_CALL(temp, Open(0, Pointee(*tempname))).
//			InSequence(s1, s2);
//
//	// Call the next Flush which takes HeapDBFile
//
//	EXPECT_CALL(temp, Close()).
//			InSequence(s1).
//			WillOnce(Return(5));
//	EXPECT_CALL(mockFile, Close()).
//			InSequence(s2).
//			WillOnce(Return(5));
//	EXPECT_CALL(rfile, Remove(Pointee(*oldname))).
//			InSequence(s2).
//			WillOnce(Return(0));
//	EXPECT_CALL(rfile, Rename(Pointee(*tempname), Pointee(*oldname))).
//			InSequence(s1, s2).
//			WillOnce(Return(0));
//	EXPECT_CALL(mockFile, Open(1, Pointee(*oldname))).
//			InSequence(s1, s2);
//
//	EXPECT_CALL(temp, GetLength()).
//			WillRepeatedly(Return(1));
//	EXPECT_CALL(mockFile, GetLength()).
//			WillRepeatedly(Return(1));
//
//	Flush(temp);
//}
//
///**
// * SortedDBFile::Flush should throw a runtime_error if Remove fails.
// */
//TEST_F(SortedDBFileTest, Flush2) {
//	StrictMock<MockPipe> in;
//	StrictMock<MockPipe> out;
//	StrictMock<MockFile> temp;
//	StrictMock<MockBigQ> q;
//	char *tempname = "tempXXXXXX";
//	char *oldname = "asdasdasd.bin";
//
//	SetIn(&in);
//	SetOut(&out);
//	SetBigQ(&q);
//
//	Sequence s1, s2;
//	EXPECT_CALL(rfile, MakeTemp(NotNull())).
//			InSequence(s1, s2);
//	EXPECT_CALL(temp, Open(0, Pointee(*tempname))).
//			InSequence(s1, s2);
//
//	// Call the next Flush which takes HeapDBFile
//
//	EXPECT_CALL(temp, Close()).
//			InSequence(s1).
//			WillOnce(Return(true));
//	EXPECT_CALL(mockFile, Close()).
//			InSequence(s2).
//			WillOnce(Return(true));
//	EXPECT_CALL(rfile, Remove(Pointee(*oldname))).
//			InSequence(s2).
//			WillOnce(Return(1));
//	EXPECT_CALL(rfile, Rename(Pointee(*tempname), Pointee(*oldname))).
//			Times(AtMost(1)).
//			InSequence(s1, s2).
//			WillRepeatedly(Return(0));
//
//	EXPECT_CALL(temp, GetLength()).
//			WillRepeatedly(Return(1));
//	EXPECT_CALL(mockFile, GetLength()).
//			WillRepeatedly(Return(1));
//
//	EXPECT_THROW(Flush(temp), std::runtime_error);
//}
//
///**
// * SortedDBFile::Flush should throw a runtime_error if Rename fails.
// */
//TEST_F(SortedDBFileTest, Flush3) {
//	StrictMock<MockPipe> in;
//	StrictMock<MockPipe> out;
//	StrictMock<MockFile> temp;
//	StrictMock<MockBigQ> q;
//	char *tempname = "tempXXXXXX";
//	char *oldname = "asdasdasd.bin";
//
//	SetIn(&in);
//	SetOut(&out);
//	SetBigQ(&q);
//
//	Sequence s1, s2;
//	EXPECT_CALL(rfile, MakeTemp(NotNull())).
//			InSequence(s1, s2);
//	EXPECT_CALL(temp, Open(0, Pointee(*tempname))).
//			InSequence(s1, s2);
//
//	// Call the next Flush which takes HeapDBFile
//
//	EXPECT_CALL(temp, Close()).
//			InSequence(s1).
//			WillOnce(Return(true));
//	EXPECT_CALL(mockFile, Close()).
//			InSequence(s2).
//			WillOnce(Return(true));
//	EXPECT_CALL(rfile, Remove(Pointee(*oldname))).
//			Times(AtMost(1)).
//			InSequence(s1, s2).
//			WillOnce(Return(0));
//	EXPECT_CALL(rfile, Rename(Pointee(*tempname), Pointee(*oldname))).
//			Times(AtMost(1)).
//			InSequence(s1, s2).
//			WillRepeatedly(Return(1));
//
//	EXPECT_CALL(temp, GetLength()).
//			WillRepeatedly(Return(1));
//	EXPECT_CALL(mockFile, GetLength()).
//			WillRepeatedly(Return(1));
//
//	EXPECT_THROW(Flush(temp), std::runtime_error);
//}
