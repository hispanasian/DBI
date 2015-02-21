#include "../include/HeapDBFileTest.h"
#include "../include/Schema.h"
#include "../include/RawFile.h"
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <string>

using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::SetArgPointee;

/**
 * PipedPage::GetfFirst should return 1 when there are records remaining.
 */
TEST(PipedPageTest, GetFirsrt1) {
	StrictMock<MockPipe> mock;
	PipedPage test = PipedPage(mock);
	Record rec;

	EXPECT_CALL(mock, Remove(&rec)).
			WillOnce(Return(1));

	EXPECT_EQ(1, test.GetFirst(&rec));
}

/**
 * PipedPage::GetFirst should return 0 when no record remains
 */
TEST(PipedPageTest, GetFirst2) {
	StrictMock<MockPipe> mock;
	PipedPage test = PipedPage(mock);
	Record rec;

	EXPECT_CALL(mock, Remove(&rec)).
			WillOnce(Return(0));

	EXPECT_EQ(0, test.GetFirst(&rec));
}

/**
 * PipedPage::Append should return 1 and add the record to the Pipe.
 */
TEST(PipedPageTest, Append) {
	StrictMock<MockPipe> mock;
	PipedPage test = PipedPage(mock);
	Record rec;

	EXPECT_CALL(mock, Insert(&rec));

	EXPECT_EQ(1, test.Append(&rec));
}

/**
 * PipedPage::EmptyItOut should shut down the internal Pipe.
 */
TEST(PipedPageTest, EmptyItOut) {
	StrictMock<MockPipe> mock;
	PipedPage test = PipedPage(mock);

	EXPECT_CALL(mock, ShutDown());

	test.EmptyItOut();
}

/**
 * A constructor that takes a file pointer should set it to NULL.
 */
TEST(PipedPageTest, Constructor) {
	StrictMock<MockPipe> mock;
	File *file;

//	EXPECT_CALL(mock, ShutDown());

	PipedPage test = PipedPage(mock, file);
}
