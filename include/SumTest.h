/*
 * SUM.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SUMTEST_H_
#define INCLUDE_SUMTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Sum.h"
#include "Pipe.h"
#include "MockClasses.h"
#include "Function.h"

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

class SumTest: public ::testing::Test {
public:
	Sum op;
	StrictMock<MockPipe> in;
	Pipe out = Pipe(1);
	StrictMock<MockFunction> fun;
	StrictMock<MockRecord> r;

	void MakeDouble(Function &fun) {
		fun.returnsInt = 0;
	}

	void MakeInt(Function &fun) {
		fun.returnsInt = 1;
	}

	int GetInt(Record &rec) {
		return ((int *)rec.bits)[2];
	}

	double GetDouble(Record &rec) {
		return *((double *) &(rec.bits[((int *)rec.bits)[1]]));
	}
};

#endif /* INCLUDE_SUMTEST_H_ */
