/*
 * PROJECTTest.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_PROJECTTEST_H_
#define INCLUDE_PROJECTTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Project.h"
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

class ProjectTest: public ::testing::Test {
public:
	Project op;
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockRecord> r;
};

#endif /* INCLUDE_PROJECTTEST_H_ */
