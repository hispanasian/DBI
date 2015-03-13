/*
 * WRITEITOUTTEST.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_WRITEITOUTTEST_H_
#define INCLUDE_WRITEITOUTTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WriteItOut.h"
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

class WriteItOutTest: public ::testing::Test {
public:
	WriteItOut op;
	FILE *f;
	StrictMock<MockPipe> in;
	StrictMock<MockRawFile> file;
	StrictMock<MockSchema> schema;
	StrictMock<MockRecord> r;
};

#endif /* INCLUDE_WRITEITOUTTEST_H_ */
