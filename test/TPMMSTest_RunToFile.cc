#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/BigQ.h"
#include "../include/TPMMSTest.h"

/**
 * With a runlen of 3, this particular test expects that RunToFile will create a run with 3 full
 * pages (ie, we do not run out of records to put into the pages, but fill the page and empty the
 * run at the same time).
 */
TEST_F(TPMMSTest, RunToFile1) {
	EXPECT_CALL(page, Append(_)).
			Times(5).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			Times(5).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			Times(5).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));
	EXPECT_CALL(file, AddPage(&page, 12));

	Record record;
	int totalPageCount = 10;
	for(int i = 0; i < 15; i++) {
		run.push_back(&record);
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(12, totalPageCount);
	EXPECT_EQ(0, run.size());
}

/**
 * With a runlen of 3, this particular test expects that RunToFile will create a run with 3 full
 * pages and 1 partially filled page (ie, we run out of records in the run before filling the fourth
 * page).
 */
TEST_F(TPMMSTest, RunToFile2) {
	EXPECT_CALL(page, Append(_)).
			Times(2).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			Times(3).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			Times(2).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			WillOnce(Return(1)); // Page still has space

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));
	EXPECT_CALL(file, AddPage(&page, 12));

	Record record;
	int totalPageCount = 10;
	for(int i = 0; i < 8; i++) {
		run.push_back(&record);
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(12, totalPageCount);
	EXPECT_EQ(0, run.size());
}

/**
 * With a runlen of 3, this particular test expects that RunToFile will create a run with 1 full
 * page and 1 partially filled page (ie, we run out of records in the run before filling the fourth
 * page).
 */
TEST_F(TPMMSTest, RunToFile3) {
	EXPECT_CALL(page, Append(_)).
			Times(2).
			WillRepeatedly(Return(1)).
			WillOnce(Return(0)).
			Times(2).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));

	Record record;
	int totalPageCount = 10;
	for(int i = 0; i < 4; i++) {
		run.push_back(&record);
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(11, totalPageCount);
	EXPECT_EQ(0, run.size());
}
