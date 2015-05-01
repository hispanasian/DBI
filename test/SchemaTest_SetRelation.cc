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
 * Schema::SetRelation should set the relation of every Attribute to the provided Schema
 */
TEST_F(SchemaTest, SetRelation1) {
	Initialize(3);
	AddAttribute("a", "rel1", Int);
	AddAttribute("b", "rel2", String);
	AddAttribute("c", "rel3", Double);

	schema.SetRelation("A");
	EXPECT_EQ(0, schema.Find("A.a"));
	EXPECT_EQ(1, schema.Find("A.b"));
	EXPECT_EQ(2, schema.Find("A.c"));
	EXPECT_EQ(Int, schema.FindType("A.a"));
	EXPECT_EQ(String, schema.FindType("A.b"));
	EXPECT_EQ(Double, schema.FindType("A.c"));
}

/**
 * Schema should not fail if there are no attributes in the relation
 */
TEST_F(SchemaTest, SetRelation2) {
	Initialize(0);

	schema.SetRelation("A");
}
