#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "BigQ.h"
#include "TPMMSTest.h"

// Adds a record to run if run is empty
TEST_F(TPMMSTest, AddRecord1) {
	MockRecord r;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(r, Size()).
		WillRepeatedly(Return(3));

	ASSERT_EQ(0, run.size());
	ASSERT_EQ(true, AddRecord(&r));
	ASSERT_EQ(1, run.size());
	ASSERT_EQ(3, GetCurrRunSizeInBytes());
	ASSERT_EQ(&r, run[0]);
}


// Will add multiple records successively
TEST_F(TPMMSTest, AddRecord2) {
	MockRecord r1;
	MockRecord r2;
	MockRecord r3;
	MockRecord r4;
	MockRecord r5;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(r1, Size()).
		WillRepeatedly(Return(1));
	EXPECT_CALL(r2, Size()).
		WillRepeatedly(Return(2));
	EXPECT_CALL(r3, Size()).
		WillRepeatedly(Return(3));
	EXPECT_CALL(r4, Size()).
		WillRepeatedly(Return(4));
	EXPECT_CALL(r5, Size()).
		WillRepeatedly(Return(5));

	ASSERT_EQ(true, AddRecord(&r1));
	ASSERT_EQ(1, run.size());
	ASSERT_EQ(&r1, run[0]);
	ASSERT_EQ(1, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(&r2));
	ASSERT_EQ(2, run.size());
	ASSERT_EQ(&r2, run[1]);
	ASSERT_EQ(3, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(&r3));
	ASSERT_EQ(3, run.size());
	ASSERT_EQ(&r3, run[2]);
	ASSERT_EQ(6, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(&r4));
	ASSERT_EQ(4, run.size());
	ASSERT_EQ(&r4, run[3]);
	ASSERT_EQ(10, GetCurrRunSizeInBytes());
	ASSERT_EQ(false, AddRecord(&r4));
	ASSERT_EQ(4, run.size());
	ASSERT_EQ(&r4, run[3]);
	ASSERT_EQ(10, GetCurrRunSizeInBytes());
}

// When the size of the record is larger than the 
// runlen, throw an exception
TEST_F(TPMMSTest, AddRecord3) {
	MockRecord r;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(r, Size()).
		WillRepeatedly(Return(11));

	EXPECT_THROW(AddRecord(&r), std::runtime_error);
	ASSERT_EQ(0, run.size());
	ASSERT_EQ(0, GetCurrRunSizeInBytes());
}

// When the record will make the run longer than
// runlen pages, don't add the record and return false
TEST_F(TPMMSTest, AddRecord4) {
	MockRecord mr;
	run.push_back(&mr);
	MockRecord r;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(9);

	EXPECT_CALL(r, Size()).
		WillRepeatedly(Return(2));

	ASSERT_EQ(false, AddRecord(&r));
	ASSERT_EQ(1, run.size());
	ASSERT_EQ(9, GetCurrRunSizeInBytes());
}