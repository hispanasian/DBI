#ifndef GROUPBYTEST_H
#define GROUPBYTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GroupBy.h"
#include "MockClasses.h"

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

class GroupByTest: public ::testing::Test {
public:
	GroupBy op;
	StrictMock<MockPipe> in;
	Pipe out = Pipe(10);
	OrderMaker groupAtts;
	StrictMock<MockFunction> func;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> prev;
	StrictMock<MockComparisonEngine> comp;

	void MakeDouble(Function &func) {
		func.returnsInt = 0;
	}

	void MakeInt(Function &func) {
		func.returnsInt = 1;
	}

	int GetInt(Record &rec) {
		return ((int *)rec.bits)[2];
	}

	double GetDouble(Record &rec) {
		return *((double *) &(rec.bits[((int *)rec.bits)[1]]));
	}
};

#endif /* GROUPBYTEST_H */