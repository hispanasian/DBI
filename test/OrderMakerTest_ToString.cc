#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/Comparison.h"
#include "../include/OrderMakerTest.h"
#include "../include/Defs.h"

/**
 * Test case of an empty OrderMaker
 */
TEST_F(OrderMakerTest, ToString1) {
	OrderMaker temp;
	order = &temp;
	SetNumAtts(0);
	string test = temp.ToString();

	EXPECT_EQ(0, strcmp("", test.c_str()));
}

/**
 * Test case for a non-empty OrderMaker with all types
 */
TEST_F(OrderMakerTest, ToString2) {
	OrderMaker temp;
	order = &temp;
	SetNumAtts(3);
	SetWhichAtt(0, 5);
	SetWhichAtt(1, 2);
	SetWhichAtt(2, 7);
	SetWhichType(0, Int);
	SetWhichType(1, Double);
	SetWhichType(2, String);
	string test = temp.ToString();

	EXPECT_EQ(0, strcmp("5 int 2 double 7 string", test.c_str()));
}
