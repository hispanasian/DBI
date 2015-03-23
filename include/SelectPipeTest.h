/*
 * SelectPipeTest.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SELECTPIPETEST_H_
#define INCLUDE_SELECTPIPETEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SelectPipe.h"
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

class SelectPipeTest: public ::testing::Test {
public:
	SelectPipe sp;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockCNF> cnf;
	StrictMock<MockRecord> literal;
	StrictMock<MockRecord> r;
};

#endif /* INCLUDE_SELECTPIPETEST_H_ */
