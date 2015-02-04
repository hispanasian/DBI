#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/DBFile.h"
#include "../include/File.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

class DBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	DBFile file = DBFile(mockFile, rfile, config);
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;

	off_t CursorIndex();
	void SetCursorIndex(off_t offset);
	off_t LastIndex();
	void SetLastIndex(off_t offset);
	File GetFile();
	Page *GetCursor();
	void SetCursor(Page &page);
	void SetCursorNull();
	Page *GetLast();
	void SetLast(Page &page);
	void SetLastNull();
	char *path = "asdasdasd";
	char *header = "asdasdasd.header";
};

off_t DBFileTest::CursorIndex() { return file.cursorIndex; }

void DBFileTest::SetCursorIndex(off_t offset) { file.cursorIndex = offset; }

off_t DBFileTest::LastIndex() { return file.lastIndex; }

void DBFileTest::SetLastIndex(off_t offset) { file.lastIndex = offset; }

File DBFileTest::GetFile() { return file.file; }

Page *DBFileTest::GetCursor() { return file.cursor; }

void DBFileTest::SetCursor(Page &page) {
	delete file.cursor;
	file.cursor = &page;
}

void DBFileTest::SetCursorNull() { file.cursor= NULL; }

Page *DBFileTest::GetLast() { return file.last; }

void DBFileTest::SetLast(Page &page) {
	delete file.last;
	file.last = &page;
}

void DBFileTest::SetLastNull() { file.last = NULL; }

/**
 * DBFile::FileExists should return false when no file named "asdasdasd" is found.
 */
TEST(DBFile, FileExists1) {
	DBFile file;
	char *path = "asdasdasd";
	EXPECT_EQ(false, FileExists(path));
}

/**
 * DBFile::FileExists should return true when a file named "asdasdasd" exists.
 */
TEST(DBFile, FileExists2) {
	DBFile file;
	char *path = "asdasdasd";
	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_EQ(true, FileExists(path));
	remove(path);
}

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's lastIndex unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=heap" in the Config file when heap is
 * passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateHeap) {
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
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(1, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	SetLastNull();
	SetCursorNull();
}

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's lastIndex unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=sorted" in the Config file when sorted
 * is passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateSorted) {
	Sequence s1, s2, s3;
	SetLast(last);
	SetCursor(cursor);

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "sorted")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());


	EXPECT_EQ(1, file.Create(path, sorted, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	SetLastNull();
	SetCursorNull();
}

/**
 * DBFile:Create should check if a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's lastIndex unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=tree" in the Config file when tree is
 * passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateTree) {
	Sequence s1, s2, s3;
	SetLast(last);
	SetCursor(cursor);

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(1, file.Create(path, tree, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	SetLastNull();
	SetCursorNull();
}

/**
 * DBFile::Create should first check if a file exists before attempting to create it. If it exists,
 * Create should exit with 0. It should also start off by clearing DBConfig. Should this return 0,
 * DBFile also needs to remove any files it created. This must be tested via integration testing.
 * However, DBFile should NOT remove any files that existed before it was run. DBFile should also
 * clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create1) {
	SetCursor(cursor);
	SetLast(last);

	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());


	// Cleanup
	remove(path);
	SetLastNull();
	SetCursorNull();
}

/**
 * DBFile::Create should first check if a header exists before attempting to create it. If it
 * exists, exists, Create should exit with 0. It should also start off by clearing DBConfig. Should
 * this return 0, DBFile should NOT remove a file that existed before it was called. DBFile should
 * also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create2) {
	SetCursor(cursor);
	SetLast(last);

	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Create should return 0 if both the header exist and path exist. It should also start off
 * by clearing DBConfig. Should this return 0. DBFile should NOT remove a file that existed before
 * it was called. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create3) {
	SetCursor(cursor);
	SetLast(last);

	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}


/**
 * DBFile::Create should return with 0 if it fails to create the header file. It should not create
 * a File should this occur. It should also start off by clearing DBConfig. Should this return 0,
 * DBFile also needs to remove any files it created. This must be tested via integration testing.
 * DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create4) {
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;
	EXPECT_CALL(config, Clear()).
			InSequence(s1, s2, s3);

	// s1
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s1).
			WillOnce(Return(false));

	// s2
	EXPECT_CALL(config, AddKey(_,_)).
			Times(AtLeast(0)).
			InSequence(s2);
	EXPECT_CALL(config, Clear()).
			InSequence(s2);

	// s3 (arbitrary call)
	EXPECT_CALL(mockFile, Open(0, path)).
			Times(AtMost(1)).
			InSequence(s3);
	EXPECT_CALL(mockFile, Close()).
			Times(AtMost(1)).
			InSequence(s3).
			WillRepeatedly(Return(true));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(path, tree, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile:Create should return 0 if config.Write returns false. If this occurs, it should remove
 * the header if it exists (if something got written) and return with a 0. It should also not
 * create the file found at path. It should also start off by clearing DBConfig. Should this return
 * 0, DBFile also needs to remove any files it created. This must be tested via integration testing.
 * DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create5) {
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
			InSequence(s1, s2, s3);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(mockFile, Open(0, path)). // Arbitrary call
			Times(AtMost(1)).
			InSequence(s2);
	EXPECT_CALL(config, AddKey("fType", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s1, s2, s3).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
			InSequence(s3);
	EXPECT_CALL(rfile, Close()).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(mockFile, Close()). // Arbitrary Call
			Times(AtMost(1)).
			InSequence(s2).
			WillRepeatedly(Return(true));

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(path, tree, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());


	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile:Create should return 0 if RawFile::Open returns false. It should also start off by
 * clearing DBConfig. Should this return 0, DBFile also needs to remove any files it created. This
 * must be tested via integration testing. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create6) {
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
			InSequence(s1, s2, s3);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(false));
	EXPECT_CALL(mockFile, Open(0, path)). // Arbitrary call
			Times(AtMost(1)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			Times(AtMost(1)).
			InSequence(s1).
			WillRepeatedly(Return(true));
	EXPECT_CALL(config, AddKey(_,_)).
			Times(AtLeast(0)).
			InSequence(s3);
	EXPECT_CALL(config, Clear()).
			InSequence(s1, s2);

	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(path, tree, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());	
	
	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Create should return 0 if the provided path is null. It should also start off by
 * clearing DBConfig. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create7) {
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(config, Clear());
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(0, file.Create(NULL, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Open should simply open a file if it exists along with it's header and return 1 if there
 * were no issues.
 */
TEST_F(DBFileTest, Open1) {
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
			WillOnce(Return(0));

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
	EXPECT_CALL(mockFile, GetPage(_, _));
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(last, EmptyItOut());

	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	// Cleanup
	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

/**
 * If the header fails to open, all files should be closed and Open should return false.
 */
TEST_F(DBFileTest, Open2) {
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
TEST_F(DBFileTest, Open3) {
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
TEST_F(DBFileTest, Open4) {
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
	EXPECT_CALL(mockFile, GetLength());
	EXPECT_CALL(mockFile, GetPage(_,_));
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
TEST_F(DBFileTest, Open5) {
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
TEST_F(DBFileTest, Open6) {
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
TEST_F(DBFileTest, Open7) {
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
 * DBFile::Close should call File.Close, write last to disk, and clear out last, cursor, lastIndex,
 * and cursorIndex. It should then return 1.
 */
TEST_F(DBFileTest, Close1) {
	SetCursorIndex(5);
	SetLastIndex(6);
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&last, 6)).
			InSequence(s3);
	EXPECT_CALL(last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());


	EXPECT_EQ(1, file.Close());
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Close should return false if DBConfig::Write fails, but it should still close RawFile,
 * clear config, zero out the indexes, and write out last.
 */
TEST_F(DBFileTest, Close2) {
	SetCursorIndex(5);
	SetLastIndex(6);
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&last, 6)).
			InSequence(s3);
	EXPECT_CALL(last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(0, file.Close());
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Close should add page to File if a record has been added to the current page. However,
 * it should also return false if RawFile::Close fails. It should also still write out the last
 * page and zero out the indexes.
 */
TEST_F(DBFileTest, Close4) {
	SetCursorIndex(5);
	SetLastIndex(6);
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(false));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&last, 6)).
			InSequence(s3);
	EXPECT_CALL(last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(0, file.Close());
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Close should write last to disk, even if last is the first page (or there are no other
 * pages).
 */
TEST_F(DBFileTest, Close5) {
	SetCursorIndex(0);
	SetLastIndex(0);
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&last, 0)).
			InSequence(s3);
	EXPECT_CALL(last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());

	EXPECT_EQ(1, file.Close());
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::MoveFirst should ask File for the first (0th) Page and put it into page. It should then
 * update lastIndex to 0. It should also write out the last page
 */
TEST_F(DBFileTest, MoveFirst1) {
	SetCursorIndex(4);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::MoveFirst should ask File for the first(0th) Page and put it into page even if the
 * current page is already 0. It should also write out the last page.
 */
TEST_F(DBFileTest, MoveFirst2) {
	SetCursorIndex(0);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is not full, add the record to last.
 */
TEST_F(DBFileTest, Add1) {
	SetCursorIndex(0);
	SetLastIndex(5);

	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	Sequence s1;
	EXPECT_CALL(last, Append(&record)).
			InSequence(s1).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is not full and cursor is on the last page, add the record to last.
 */
TEST_F(DBFileTest, Add2) {
	SetCursorIndex(5);
	SetLastIndex(5);

	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	Sequence s1;
	EXPECT_CALL(last, Append(&record)).
			InSequence(s1).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is full, write out the last page and add the record to the cleared
 * last.
 */
TEST_F(DBFileTest, Add3) {
	SetCursorIndex(0);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, AddPage(&last, 5));
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(6, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, even if the last page is the current page, write the record to last.
 */
TEST_F(DBFileTest, Add4) {
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, even if the last page is the first page AND the current page, write the
 * record to last.
 */
TEST_F(DBFileTest, Add5) {
	SetCursorIndex(0);
	SetLastIndex(0);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());

	SetCursorNull();
	SetLastNull();
}

/**
 * DBFile::Add should, if last is full and cursor is at the end, write out the last page and add
 * the record to the cleared last.
 */
TEST_F(DBFileTest, Add6) {
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, AddPage(&last, 5));
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(6, LastIndex());

	SetCursorNull();
	SetLastNull();
}
