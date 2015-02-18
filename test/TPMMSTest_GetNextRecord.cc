#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/BigQ.h"
#include "../include/TPMMSTest.h"

/**
 * If the replacement of min exists and is in the same page, replace the
 * current min with it.
 */
TEST_F(TPMMSTest, GetNextRecord1) {
	Record *temp[5];
	Record **heads = temp;
	off_t temp2[5];
	off_t *runIndex = temp2;
	Page *temp3[5];
	Page **pages = temp3;
	StrictMock<Record> temp4;
	Record *nextMin = &temp4;

	int runsLeft = 3;

	StrictMock<MockRecord> head0;
	StrictMock<MockRecord> head1;
	StrictMock<MockRecord> head2;
	StrictMock<MockRecord> head3;
	StrictMock<MockRecord> head4;

	heads[0] = &head0;
	heads[1] = &head1;
	heads[2] = &head2;
	heads[3] = &head3;
	heads[4] = &head4;

	StrictMock<MockPage> minPage;

	pages[0] = NULL;
	pages[1] = NULL;
	pages[2] = NULL;
	pages[3] = &minPage;
	pages[4] = NULL;

	EXPECT_CALL(minPage, GetFirst(&head3)).
			WillOnce(DoAll(SetArgPointee<0>(*nextMin), Return(1)));

	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	EXPECT_EQ(3, runsLeft);
	EXPECT_EQ(&(*nextMin), &(*heads[3]));
}

/**
 * If the replacement of min exists in the next page, get the next page and
 * increment the min in runIndex
 */
TEST_F(TPMMSTest, GetNextRecord2) {

}

/**
 * If the replacement of min does not exist, decrement runsLeft and replace
 * the min in heads with NULL.
 */
TEST_F(TPMMSTest, GetNextRecord3) {

}
