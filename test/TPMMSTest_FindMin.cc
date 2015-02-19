#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/BigQ.h"
#include "../include/TPMMSTest.h"

/**
 * TPMMS::FindMin should return -1 if no min can be found (ie, every element in head is null)
 */
TEST_F(TPMMSTest, FindMin1) {
	Record *temp[5];
	Record **head = temp;
	for(int i = 0; i < 5; i++) { temp[i] = NULL; }

	EXPECT_EQ(-1, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return 0 if the min element is at 0
 */
TEST_F(TPMMSTest, FindMin2) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = &rec; }

	Sequence s1;
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(4).
			InSequence(s1).
			WillRepeatedly(Return(5));

	EXPECT_EQ(0, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return 4 if the last element (of 5 elements) is min
 */
TEST_F(TPMMSTest, FindMin3) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = &rec; }

	Sequence s1;
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(3).
			InSequence(s1).
			WillRepeatedly(Return(-1));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			InSequence(s1).
			WillOnce(Return(-1));


	EXPECT_EQ(4, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return 2 if the middle element (of 5 elements) is min
 */
TEST_F(TPMMSTest, FindMin4) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = &rec; }

	Sequence s1;
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(1).
			InSequence(s1).
			WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			InSequence(s1).
			WillOnce(Return(-1));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(2));


	EXPECT_EQ(2, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return the only element that is not NULL if all but one elements are NULL
 */
TEST_F(TPMMSTest, FindMin5) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = NULL; }
	temp[0] = &rec;

	EXPECT_EQ(0, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return the only element that is not NULL if all but one elements are NULL
 */
TEST_F(TPMMSTest, FindMin6) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = NULL; }
	temp[2] = &rec;

	EXPECT_EQ(2, FindMin(5,head));
}

/**
 * TPMMS::FindMin should return the only element that is not NULL if all but one elements are NULL
 */
TEST_F(TPMMSTest, FindMin7) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = NULL; }
	temp[4] = &rec;

	EXPECT_EQ(4, FindMin(5,head));
}

/**
 * TPMMS::FindMin should ignore some NULL elements
 */
TEST_F(TPMMSTest, FindMin8) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = &rec; }
	temp[0] = NULL;
	temp[3] = NULL;

	Sequence s1;
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			InSequence(s1).
			WillOnce(Return(-1));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			InSequence(s1).
			WillOnce(Return(-1));

	EXPECT_EQ(4, FindMin(5,head));
}
