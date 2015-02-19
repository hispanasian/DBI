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
	MockRecord mockNextMin;

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
			WillOnce(Return(1));
	
	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	EXPECT_EQ(3, runsLeft);
	EXPECT_EQ(&head3, heads[3]);
}

/**
 * If the replacement of min exists in the next page, get the next page and
 * increment the min in runIndex
 */
TEST_F(TPMMSTest, GetNextRecord2) {
	Record *temp[5];
	Record **heads = temp;
	off_t temp2[5];
	off_t *runIndex = temp2;
	Page *temp3[5];
	Page **pages = temp3;

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

	runPos.push_back(0);
	runPos.push_back(2);
	runPos.push_back(5);
	runPos.push_back(7);
	runPos.push_back(9);
	runPos.push_back(10);
	runIndex[3] = 7;

	StrictMock<MockPage> minPage;
	pages[0] = NULL;
	pages[1] = NULL;
	pages[2] = NULL;
	pages[3] = &minPage;
	pages[4] = NULL;

	
	{
		InSequence seq;
		EXPECT_CALL(minPage, GetFirst(heads[3])).
			WillOnce(Return(0));

		EXPECT_CALL(file, GetPage(pages[3], 8));

		EXPECT_CALL(minPage, GetFirst(heads[3])).
			WillOnce(Return(1));
	}
	

	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	ASSERT_EQ(3, runsLeft);
	ASSERT_EQ(8, runIndex[3]);
}

/**
 * If the replacement of min does not exist, decrement runsLeft and replace
 * the min in heads with NULL.
 */
TEST_F(TPMMSTest, GetNextRecord3) {
	Record *temp[5];
	Record **heads = temp;
	off_t temp2[5];
	off_t *runIndex = temp2;
	Page *temp3[5];
	Page **pages = temp3;

	int runsLeft = 3;

	StrictMock<MockRecord> head0;
	StrictMock<MockRecord> head1;
	StrictMock<MockRecord> head2;
	StrictMock<MockRecord> *head3= new StrictMock<MockRecord>;
	StrictMock<MockRecord> head4;

	heads[0] = &head0;
	heads[1] = &head1;
	heads[2] = &head2;
	heads[3] = head3;
	heads[4] = &head4;

	runPos.push_back(0);
	runPos.push_back(2);
	runPos.push_back(5);
	runPos.push_back(7);
	runPos.push_back(9);
	runPos.push_back(10);
	runIndex[3] = 9;

	StrictMock<MockPage> minPage;
	pages[0] = NULL;
	pages[1] = NULL;
	pages[2] = NULL;
	pages[3] = &minPage;
	pages[4] = NULL;

	EXPECT_CALL(minPage, GetFirst(heads[3])).
		WillOnce(Return(0));

	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	ASSERT_EQ(2, runsLeft);
	ASSERT_EQ(NULL, heads[3]);
}
