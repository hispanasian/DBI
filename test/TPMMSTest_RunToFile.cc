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
	Sequence s1;
	EXPECT_CALL(page, Append(_)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));
	EXPECT_CALL(file, AddPage(&page, 12));

	off_t totalPageCount = 10;
	for(int i = 0; i < 15; i++) {
		run.push_back(new Record());
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(13, totalPageCount);
	EXPECT_EQ(0, run.size());
}

/**
 * With a runlen of 3, this particular test expects that RunToFile will create a run with 3 full
 * pages and 1 partially filled page (ie, we run out of records in the run before filling the fourth
 * page).
 */
TEST_F(TPMMSTest, RunToFile2) {
	Sequence s1;
	EXPECT_CALL(page, Append(_)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			Times(3).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));
	EXPECT_CALL(file, AddPage(&page, 12));
	EXPECT_CALL(file, AddPage(&page, 13));

	off_t totalPageCount = 10;
	for(int i = 0; i < 8; i++) {
		run.push_back(new Record());
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(14, totalPageCount);
	EXPECT_EQ(0, run.size());
}

/**
 * With a runlen of 3, this particular test expects that RunToFile will create a run with 1 full
 * page and 1 partially filled page (ie, we run out of records in the run before filling the fourth
 * page).
 */
TEST_F(TPMMSTest, RunToFile3) {
	Sequence s1;
	EXPECT_CALL(page, Append(_)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(_)).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(_)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));

	off_t totalPageCount = 10;
	for(int i = 0; i < 4; i++) {
		run.push_back(new Record());
	}

	RunToFile(totalPageCount);
	EXPECT_EQ(12, totalPageCount);
	EXPECT_EQ(0, run.size());
}
