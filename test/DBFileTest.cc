#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "DBFile.h"

using ::testing::_;

class DBFileTest: public ::testing::Test {
public:
	MockFile mockFile;
	MockDBConfig config;
	MockRawFile rfile;
	DBFile file = DBFile(mockFile, rfile, config);
	off_t CurPage();
	File GetFile();
};

off_t DBFileTest::CurPage() { return file.curPage; }

File DBFileTest::GetFile() { return file.file; }

/**
 * DBFile:Create should check of a file called "asdasdasd" exists (and it should not). it should
 * then call Create on file and leave it's curPage unchanged. It should also create a header called
 * "asdasdasd.header"
 */
TEST_F(DBFileTest, Create1) {
	EXPECT_CALL(mock, Create("asdasdasd"));

	// Remove the file called "asdasdasd" and "asdasdasd.header"
	FILE
}
