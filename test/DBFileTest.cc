#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "MockClasses.h"
#include "DBFile.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;

class DBFileTest: public ::testing::Test {
public:
	MockFile mockFile;
	MockDBConfig config;
	MockRawFile rfile;
	DBFile file = DBFile(mockFile, rfile, config);
	off_t CurPage();
	File GetFile();
	char *path = "asdasdasd";
	char *header = "asdasdasd.header";
};

off_t DBFileTest::CurPage() { return file.curPage; }

File DBFileTest::GetFile() { return file.file; }

/**
 * DBFile::FileExists should return false when no file named "asdasdasd" is found.
 */
TEST(DBFile, FileExists1) {
	DBFile file;
	char *path = "asdasdasd";
	EXPECT_EQ(false, file.FileExists(path));
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
	EXPECT_EQ(true, file.FileExists(path));
	remove(path);
}

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "ftype=heap" in the Config file when heap is
 * passed as the ftype.
 */
TEST_F(DBFileTest, CreateHeap) {
	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2);
	EXPECT_CALL(config, AddKey("ftype", "heap")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(1, file.Create(path, heap, NULL));
	EXPECT_EQ(true, file.FileExists(path));
	EXPECT_EQ(true, file.FileExists(header));
}

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "ftype=sorted" in the Config file when sorted
 * is passed as the ftype.
 */
TEST_F(DBFileTest, CreateStorted) {
	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2);
	EXPECT_CALL(config, AddKey("ftype", "sorted")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(1, file.Create(path, sorted, NULL));
	EXPECT_EQ(true, file.FileExists(path));
	EXPECT_EQ(true, file.FileExists(header));

}

/**
 * DBFile:Create should check if a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "ftype=tree" in the Config file when tree is
 * passed as the ftype.
 */
TEST_F(DBFileTest, CreateTree) {
	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2);
	EXPECT_CALL(config, AddKey("ftype", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(1, file.Create(path, tree, NULL));
}

/**
 * DBFile::Create should first check if a file exists before attempting to create it. If it exists,
 * exists, Create should exit with 0
 */
TEST_F(DBFileTest, Create1) {
	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(path);
}

/**
 * DBFile::Create should first check if a header exists before attempting to create it. If it exists,
 * exists, Create should exit with 0
 */
TEST_F(DBFileTest, Create2) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(header);
}

/**
 * DBFile::Create should return 0 if both the header exist and path exist.
 */
TEST_F(DBFileTest, Create3) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(path);
	remove(header);
}

/**
 * DBFile::Create should return with 0 if mockFile.Close returns anything greater than 0. Create
 * should not create a header and should remove any created files. Note that This must be
 * integration tested as we can't test the removal of a file and it is in fact, not a problem if
 * DBFile attempts to write a header at some point as long as it is removed.
 */
TEST_F(DBFileTest, Create4) {
	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2);
	EXPECT_CALL(config, AddKey("ftype", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile::Create should return with 0 if it fails to create the header file. It should not create
 * a File should this occur.
 */
TEST_F(DBFileTest, Create5) {
	Sequence s1, s2, s3;

	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(false));

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile:Create should return 0 if config.Write returns false. If this occurs, it should remove
 * the header if it exists (if something got written) and return with a 0. It should also not
 * create the file found at path.
 */
TEST_F(DBFileTest, Create6) {
	Sequence s1, s2, s3;

	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("ftype", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(false));

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile:Create should return 0 if RawFile::Close returns false.
 */
TEST_F(DBFileTest, Create7) {
	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("ftype", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(false));

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile::Create should return 0 if the provided path is null
 */
TEST_F(DBFileTest, Create8) {
	EXPECT_EQ(0, file.Create(NULL, heap, NULL));
}
