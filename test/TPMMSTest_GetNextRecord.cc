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

	GetNextRecord(3, heads, runIndex, runsLeft);
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
