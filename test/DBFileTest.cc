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
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=heap" in the Config file when heap is
 * passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateHeap) {
	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
			InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "heap")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(1, file.Create(path, heap, NULL));
}

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=sorted" in the Config file when sorted
 * is passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateStorted) {
	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "sorted")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_EQ(1, file.Create(path, sorted, NULL));

}

/**
 * DBFile:Create should check if a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header". It should also make a key for "fType=tree" in the Config file when tree is
 * passed as the fType. It should also start off by clearing DBConfig.
 */
TEST_F(DBFileTest, CreateTree) {
	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "tree")).
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
 * Create should exit with 0. It should also start off by clearing DBConfig. Should this return 0,
 * DBFile also needs to remove any files it created. This must be tested via integration testing.
 * However, DBFile should NOT remove any files that existed before it was run. DBFile should also
 * clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create1) {
	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	InSequence seq;
	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(path);
}

/**
 * DBFile::Create should first check if a header exists before attempting to create it. If it exists,
 * exists, Create should exit with 0. It should also start off by clearing DBConfig. Should this
 * return 0, DBFile should NOT remove a file that existed before it was called. DBFile should also
 * clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create2) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	InSequence seq;
	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(header);
}

/**
 * DBFile::Create should return 0 if both the header exist and path exist. It should also start off
 * by clearing DBConfig. Should this return 0. DBFile should NOT remove a file that existed before
 * it was called. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create3) {
	FILE *temp = fopen(header, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	temp = fopen(path, "w");
	fprintf(temp, "stuff");

	InSequence seq;
	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(path, heap, NULL));

	// Cleanup
	remove(path);
	remove(header);
}


/**
 * DBFile::Create should return with 0 if it fails to create the header file. It should not create
 * a File should this occur. It should also start off by clearing DBConfig. Should this return 0,
 * DBFile also needs to remove any files it created. This must be tested via integration testing.
 * DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create4) {
	InSequence seq;
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(rfile, Open(header)).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile:Create should return 0 if config.Write returns false. If this occurs, it should remove
 * the header if it exists (if something got written) and return with a 0. It should also not
 * create the file found at path. It should also start off by clearing DBConfig. Should this return
 * 0, DBFile also needs to remove any files it created. This must be tested via integration testing.
 * DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create5) {
	Sequence s1, s2;

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "tree")).
			InSequence(s1);
	EXPECT_CALL(config, Write(_)).
			InSequence(s1, s2).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
					InSequence(s1, s2);

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile:Create should return 0 if RawFile::Close returns false. It should also start off by
 * clearing DBConfig. Should this return 0, DBFile also needs to remove any files it created. This
 * must be tested via integration testing. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create6) {
	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "tree")).
			InSequence(s3);
	EXPECT_CALL(config, Write(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2, s3).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
					InSequence(s1, s2, s3);

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile:Create should return 0 if RawFile::Open returns false. It should also start off by
 * clearing DBConfig. Should this return 0, DBFile also needs to remove any files it created. This
 * must be tested via integration testing. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create7) {
	Sequence s1, s2, s3;

	EXPECT_CALL(config, Clear()).
				InSequence(s1, s2, s3);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(false));
	EXPECT_CALL(config, Clear()).
					InSequence(s1, s2, s3);

	EXPECT_EQ(0, file.Create(path, tree, NULL));
}

/**
 * DBFile::Create should return 0 if the provided path is null. It should also start off by
 * clearing DBConfig. DBFile should also clear config again if it returns 0.
 */
TEST_F(DBFileTest, Create8) {
	InSequence seq;
	EXPECT_CALL(config, Clear());
	EXPECT_EQ(0, file.Create(NULL, heap, NULL));
}
