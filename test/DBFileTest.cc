#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"

using ::testing::_;

class DBFileTest: public ::testing::Test {
public:
	DBFile file;
	off_t CurPage();
	File GetFile();
};

off_t DBFileTest::CurPage() { return file.curPage; }

File DBFileTest::GetFile() { return file.file; }

TEST_F(DBFileTest, Create1) {

}
