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

TEST_F(RecordTest, ConstructorInt) {
	int i = 51;
	Record r = Record(i);
	std::string out = "i: [51]\n";
	Schema schema;
	IntSchema(schema);

	EXPECT_EQ(0, out.compare(r.ToString(&schema)));
	ClearSchema(schema);
}

TEST_F(RecordTest, ConstructorDouble) {
	double d = 25.5;
	Record r = Record(d);
	std::string out = "d: [25.500000]\n";
	Schema schema;
	DoubleSchema(schema);

	EXPECT_EQ(0, out.compare(r.ToString(&schema)));
	ClearSchema(schema);
}
