/*
 * SelectPipeTest.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_DUPLICATEREMOVALTEST_H_
#define INCLUDE_DUPLICATEREMOVALTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "DuplicateRemoval.h"
#include "MockClasses.h"
#include "Comparison.h"

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

class DuplicateRemovalTest: public ::testing::Test {
public:
	DuplicateRemoval op;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockSchema> schema;
	OrderMaker order;
	StrictMock<MockRecord> r;
	StrictMock<MockRecord> prev;
};

#endif /* INCLUDE_DUPLICATEREMOVALTEST_H_ */
