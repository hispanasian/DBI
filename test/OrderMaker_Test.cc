#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/Comparison.h"
#include "../include/OrderMakerTest.h"

/**
 * Test case of empty string
 */
TEST_F(OrderMakerTest, Constructor1) {
	OrderMaker temp = OrderMaker("");
	order = &temp;

	EXPECT_EQ(0, GetNumAtts());
}


/**
 * Test case of single pair
 */
TEST_F(OrderMakerTest, Constructor2) {
	OrderMaker temp = OrderMaker("0 int");
	order = &temp;

	EXPECT_EQ(1, GetNumAtts());
	EXPECT_EQ(0, GetWhichAtt(0));
	EXPECT_EQ(Int, GetWhichType(0));
}

/**
 * Test case of multiple pairs with all types
 */
TEST_F(OrderMakerTest, Constructor3) {
	OrderMaker temp = OrderMaker("0 int 12 string 5 double");
	order = &temp;

	EXPECT_EQ(3, GetNumAtts());
	EXPECT_EQ(0, GetWhichAtt(0));
	EXPECT_EQ(12, GetWhichAtt(1));
	EXPECT_EQ(5, GetWhichAtt(2));
	EXPECT_EQ(Int, GetWhichType(0));
	EXPECT_EQ(String, GetWhichType(1));
	EXPECT_EQ(Double, GetWhichType(2));
}

/**
 * Test case with missing attribute in pair should throw a invalid_argument exception
 */
TEST_F(OrderMakerTest, Constructor4) {
	EXPECT_THROW(OrderMaker("int"), std::invalid_argument);
}

/**
 * Test case with missing type in pair should throw a invalid_argument exception
 */
TEST_F(OrderMakerTest, Constructor5) {
	EXPECT_THROW(OrderMaker("0"), std::invalid_argument);
}

/**
 * Test case with non-integer attribute should throw a invalid_argument exception
 */
TEST_F(OrderMakerTest, Constructor6) {
	EXPECT_THROW(OrderMaker("a int"), std::invalid_argument);
}

/**
 * Test case with non-Type type should throw a invalid_argument exception
 */
TEST_F(OrderMakerTest, Constructor7) {
	EXPECT_THROW(OrderMaker("0 a"), std::invalid_argument);
}
