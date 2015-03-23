#ifndef RECORDTEST_H
#define RECORDTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "Record.h"
#include "Schema.h"

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

class RecordTest: public ::testing::Test {
public:


	void DoubleSchema(Schema &schema) {
		schema.numAtts = 1;
		schema.myAtts = new Attribute[1];
		schema.myAtts[0].name = "d";
		schema.myAtts[0].myType = Double;
	}

	void IntSchema(Schema &schema) {
		schema.numAtts = 1;
		schema.myAtts = new Attribute[1];
		schema.myAtts[0].name = "i";
		schema.myAtts[0].myType = Int;
	}

	/* This exists because free() is a pain in the butt */
	void ClearSchema(Schema &schema) {
		schema.myAtts[0].name = new char[1];
	}
};

#endif
