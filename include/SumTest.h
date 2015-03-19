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

class SumTest: public ::testing::Test {
public:
	Sum op;
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFunction> fun;
	StrictMock<MockRecord> r;
};

#endif /* INCLUDE_SUMTEST_H_ */
