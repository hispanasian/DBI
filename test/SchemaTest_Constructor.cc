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
 * Schema(Schema, vector<RelAttPair>) should create a Schema that contains the subset of the
 * provided schema according to the vector (and in order of the vector)
 */
TEST_F(SchemaTest, Constructor1) {
	Initialize(5);
	AddAttribute("a", "A", Int);
	AddAttribute("b", "A", Int);
	AddAttribute("a", "B", Double);
	AddAttribute("b", "B", Double);
	AddAttribute("a", "C", String);

	vector<RelAttPair> temp;
	temp.push_back(RelAttPair("A", "b"));
	temp.push_back(RelAttPair("C", "a"));
	temp.push_back(RelAttPair("B", "a"));

	Schema subset (schema, temp);

	EXPECT_EQ(3, subset.GetNumAtts());
	EXPECT_EQ(0, subset.Find("A.b"));
	EXPECT_EQ(1, subset.Find("C.a"));
	EXPECT_EQ(2, subset.Find("B.a"));
	EXPECT_EQ(Int, subset.FindType("A.b"));
	EXPECT_EQ(String, subset.FindType("C.a"));
	EXPECT_EQ(Double, subset.FindType("B.a"));
}

/**
 * Schema(Schema, vector<RelAttPair>) should not fail if there are no attributes in the vector
 */
TEST_F(SchemaTest, Constructor2) {
	Initialize(5);
	AddAttribute("a", "A", Int);
	AddAttribute("b", "A", Int);
	AddAttribute("a", "B", Double);
	AddAttribute("b", "B", Double);
	AddAttribute("a", "C", String);

	vector<RelAttPair> temp;

	Schema subset (schema, temp);

	EXPECT_EQ(0, subset.GetNumAtts());
}

/**
 * Schema(Schema, vector<RelAttPair>) should not fail if the entire Schema is referenced in the
 * vector
 */
TEST_F(SchemaTest, Constructor3) {
	Initialize(5);
	AddAttribute("a", "A", Int);
	AddAttribute("b", "A", Int);
	AddAttribute("a", "B", Double);
	AddAttribute("b", "B", Double);
	AddAttribute("a", "C", String);

	vector<RelAttPair> temp;
	temp.push_back(RelAttPair("A", "a"));
	temp.push_back(RelAttPair("A", "b"));
	temp.push_back(RelAttPair("B", "a"));
	temp.push_back(RelAttPair("B", "b"));
	temp.push_back(RelAttPair("C", "a"));

	Schema subset (schema, temp);

	EXPECT_EQ(5, subset.GetNumAtts());
	EXPECT_EQ(0, subset.Find("A.a"));
	EXPECT_EQ(1, subset.Find("A.b"));
	EXPECT_EQ(2, subset.Find("B.a"));
	EXPECT_EQ(3, subset.Find("B.b"));
	EXPECT_EQ(4, subset.Find("C.a"));

	EXPECT_EQ(Int, subset.FindType("A.a"));
	EXPECT_EQ(Int, subset.FindType("A.b"));
	EXPECT_EQ(String, subset.FindType("C.a"));
	EXPECT_EQ(Double, subset.FindType("B.a"));
	EXPECT_EQ(Double, subset.FindType("B.b"));
}

/**
 * Schema(Schema, vector<RelAttPair>) should not fail if the entire Schema is referenced in the
 * vector in reversed order
 */
TEST_F(SchemaTest, Constructor4) {
	Initialize(5);
	AddAttribute("a", "A", Int);
	AddAttribute("b", "A", Int);
	AddAttribute("a", "B", Double);
	AddAttribute("b", "B", Double);
	AddAttribute("a", "C", String);

	vector<RelAttPair> temp;
	temp.push_back(RelAttPair("C", "a"));
	temp.push_back(RelAttPair("B", "b"));
	temp.push_back(RelAttPair("B", "a"));
	temp.push_back(RelAttPair("A", "b"));
	temp.push_back(RelAttPair("A", "a"));

	Schema subset (schema, temp);

	EXPECT_EQ(5, subset.GetNumAtts());
	EXPECT_EQ(4, subset.Find("A.a"));
	EXPECT_EQ(3, subset.Find("A.b"));
	EXPECT_EQ(2, subset.Find("B.a"));
	EXPECT_EQ(1, subset.Find("B.b"));
	EXPECT_EQ(0, subset.Find("C.a"));

	EXPECT_EQ(Int, subset.FindType("A.a"));
	EXPECT_EQ(Int, subset.FindType("A.b"));
	EXPECT_EQ(String, subset.FindType("C.a"));
	EXPECT_EQ(Double, subset.FindType("B.a"));
	EXPECT_EQ(Double, subset.FindType("B.b"));
}

/**
 * Schema(Schema, vector<RelAttPair>) should not fail if the vector references a relation/attribute
 * pair that does not exist in the schema.
 */
TEST_F(SchemaTest, Constructor5) {
	Initialize(5);
	AddAttribute("a", "A", Int);
	AddAttribute("b", "A", Int);
	AddAttribute("a", "B", Double);
	AddAttribute("b", "B", Double);
	AddAttribute("a", "C", String);

	vector<RelAttPair> temp;
	temp.push_back(RelAttPair("A", "b"));
	temp.push_back(RelAttPair("C", "b"));
	temp.push_back(RelAttPair("B", "a"));

	EXPECT_THROW(Schema subset (schema, temp), std::invalid_argument);
}
