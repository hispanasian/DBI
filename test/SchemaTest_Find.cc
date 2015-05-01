#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SchemaTest.h"
#include "Schema.h"
#include "MockClasses.h"
#include "Defs.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

/**
 * Schema::Find should be able to find an attribute even if the relation is not included (rel.att)
 */
TEST_F(SchemaTest, Find1) {
	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel3", Double);

	EXPECT_EQ(0, schema.Find("a"));
	EXPECT_EQ(1, schema.Find("b"));
	EXPECT_EQ(2, schema.Find("c"));
	EXPECT_EQ(Int, schema.FindType("a"));
	EXPECT_EQ(String, schema.FindType("b"));
	EXPECT_EQ(Double, schema.FindType("c"));
}

/**
 * Schema should not report an attribute that does not exist
 */
TEST_F(SchemaTest, Find2) {
	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel3", Double);

	EXPECT_EQ(-1, schema.Find("rel1"));
}

/**
 * Schema should be able to handle the case where a string of the format "rel.att" is provided
 * and use the relation in the lookup
 */
TEST_F(SchemaTest, Find3) {
	string x = "x";
	string y = "y";
	cout << x.append(".").append(y) << endl;

	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel2", Double);

	EXPECT_EQ(0, schema.Find("rel1.a"));
	EXPECT_EQ(1, schema.Find("rel2.b"));
	EXPECT_EQ(2, schema.Find("rel2.c"));
	EXPECT_EQ(Int, schema.FindType("rel1.a"));
	EXPECT_EQ(String, schema.FindType("rel2.b"));
	EXPECT_EQ(Double, schema.FindType("rel2.c"));
}

/**
 * If an existing rel is qualified but the attribute does not exist, schema should not find it
 */
TEST_F(SchemaTest, Find4) {
	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel3", Double);

	EXPECT_EQ(-1, schema.Find("rel1.c"));
}

/**
 * if an existing att is provided but the corresponding relation is not, this should not find it
 */
TEST_F(SchemaTest, Find) {
	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel3", Double);

	EXPECT_EQ(-1, schema.Find("rel4.b"));
}
