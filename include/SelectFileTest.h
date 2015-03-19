/*
 * SelectFileTest.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SELECTFILETEST_H_
#define INCLUDE_SELECTFILETEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SelectFile.h"
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

class SelectFileTest: public ::testing::Test {
public:
	SelectFile op;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockDBFile> in;
	StrictMock<MockPipe> out;
	StrictMock<MockCNF> cnf;
	StrictMock<MockRecord> literal;
	StrictMock<MockRecord> r;
};

#endif /* INCLUDE_SELECTPIPETEST_H_ */
