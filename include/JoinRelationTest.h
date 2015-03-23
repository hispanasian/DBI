/*
 * JoinRelationTest.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_JOINRELATIONTEST_H_
#define INCLUDE_JOINRELATIONTEST_H_

#include "JoinRelation.h"

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
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;


class JoinRelationTest: public ::testing::Test {
public:
	int memLimit = 100;
	JoinRelation rel = JoinRelation(memLimit);

	void SetMemory(bool val) { rel.memory = val; }
	void SetDelegate(Relation *del) { rel.delegate = del; };
	Relation *GetDelegate() { return rel.delegate; };
};

#endif /* INCLUDE_JOINRELATIONTEST_H_ */
