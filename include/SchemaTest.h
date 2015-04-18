/*
 * SchemaTest.h
 *
 *  Created on: Apr 17, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SCHEMATEST_H_
#define INCLUDE_SCHEMATEST_H_

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

class SchemaTest: public::testing::Test {
public:
	Schema schema;
	int position = 0;
	int size = 0;

	void Initialize(int size) {
		this->size = size;
		schema.numAtts = size;
		schema.myAtts = new Attribute[size];
	}

	void AddAttribute(std::string name, std::string relation, Type type) {
		schema.myAtts[position].name = name;
		schema.myAtts[position].relation = relation;
		schema.myAtts[position].myType = type;
		++position;
	}
};



#endif /* INCLUDE_SCHEMATEST_H_ */
