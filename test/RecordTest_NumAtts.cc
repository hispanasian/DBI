#include "RecordTest.h"

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

using namespace std;

TEST_F(RecordTest, NumAtts1) {
	int i = 51;
	Record r = Record(i);

	EXPECT_EQ(1, r.NumAtts());
}

TEST_F(RecordTest, NumAtts2) {
	double d = 25.5;
	Record r = Record(d);
	EXPECT_EQ(1, r.NumAtts());
}
