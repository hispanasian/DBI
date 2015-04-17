#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/Comparison.h"
#include "../include/OrderMakerTest.h"
#include "../include/Schema.h"
#include "../include/Defs.h"

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

/**
 * OrderMaker should correctly create an order given a base schema and a schema of attributes on
 * which to sort.
 */
TEST_F(OrderMakerTest, Constructor8) {
	Attribute schemasAtts[5];
	schemasAtts[0] = Attribute{ "a1", "A", Int };
	schemasAtts[1] = Attribute{ "a2", "A", Int };
	schemasAtts[2] = Attribute{ "a1", "B", Double };
	schemasAtts[3] = Attribute{ "a2", "B", Double };
	schemasAtts[4] = Attribute{ "c1", "C", String };

	Attribute ordersAtts[3];
	ordersAtts[0] = Attribute{ "a2", "B", Double };
	ordersAtts[1] = Attribute{ "c1", "C", String };
	ordersAtts[2] = Attribute{ "a2", "A", Int };

	Schema schema ("", 5, schemasAtts);
	Schema order ("", 3, ordersAtts);

	OrderMaker test = OrderMaker(&schema, &order);
	EXPECT_EQ(0, test.ToString().compare("3 double 4 string 1 int"));
}

/**
 * OrderMaker should correctly create an order given a base schema and a schema of attributes on
 * which to sort. It should not fail of the sorting schema is empty
 */
TEST_F(OrderMakerTest, Constructor9) {
	Attribute schemasAtts[5];
	schemasAtts[0] = Attribute{ "a1", "A", Int };
	schemasAtts[1] = Attribute{ "a2", "A", Int };
	schemasAtts[2] = Attribute{ "b1", "B", Double };
	schemasAtts[3] = Attribute{ "b2", "B", Double };
	schemasAtts[4] = Attribute{ "c1", "C", String };

	Attribute ordersAtts[0];

	Schema schema ("", 5, schemasAtts);
	Schema order ("", 0, ordersAtts);

	OrderMaker test = OrderMaker(&schema, &order);
	EXPECT_EQ(0, test.ToString().compare(""));
}

/**
 * OrderMaker should throw an error if an attempt is made to create an OrderMaker given two
 * schemas with differeing values (ie, the values in the order schema are not in the base schema).
 */
TEST_F(OrderMakerTest, Constructor10) {
	Attribute schemasAtts[5];
	schemasAtts[0] = Attribute{ "a1", "A", Int };
	schemasAtts[1] = Attribute{ "a2", "A", Int };
	schemasAtts[2] = Attribute{ "b1", "B", Double };
	schemasAtts[3] = Attribute{ "b2", "B", Double };
	schemasAtts[4] = Attribute{ "c1", "C", String };

	Attribute ordersAtts[3];
	ordersAtts[0] = Attribute{ "b2", "B", Double };
	ordersAtts[1] = Attribute{ "c1", "C", String };
	ordersAtts[2] = Attribute{ "a2", "D", Int };

	Schema schema ("", 5, schemasAtts);
	Schema order ("", 3, ordersAtts);

	EXPECT_THROW(OrderMaker(&schema, &order), std::invalid_argument);
}
