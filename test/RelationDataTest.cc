#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "RelationData.h"
#include "Schema.h"
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
using ::testing::SetArgReferee;
using ::testing::DoAll;

TEST(RelationDataTest, Copy) {
	RelationData data;
	Attribute att[1];
	att[0].relation = "A"; att[0].name = "att"; att[0].myType = Double;
	Schema temp("", 1, att);
	data.Insert("A", "dir/A.db", "schemas", temp);

	RelationData copy(data);
	EXPECT_EQ(0, data["A"].dbLocation.compare("dir/A.db"));
	EXPECT_EQ(0, data["A"].schemaLocation.compare("schemas"));
	EXPECT_EQ(1, data["A"].schema.GetNumAtts());
	EXPECT_EQ(0, data["A"].schema.Find("A.att"));
	EXPECT_EQ(Double, data["A"].schema.FindType("A.att"));
}
