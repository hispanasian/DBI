/*
 * InMemoryRelationTest.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_INMEMORYRELATIONTEST_H_
#define INCLUDE_INMEMORYRELATIONTEST_H_

#include "InMemoryRelation.h"

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


class InMemoryRelationTest: public ::testing::Test {
public:
	int memLimit = 100;
	InMemoryRelation rel = InMemoryRelation(memLimit);

	int GetCount() { return rel.count; }
	void SetCount(int count) { rel.count = count; }
	int GetMemUsed() { return rel.memUsed; }
	void SetMemUsed(int memUsed) { rel.memUsed = memUsed; }
	std::vector<Record*> &GetRelation() { return rel.relation; }
	int GetIndex() { return rel.index; }
	void SetIndex(int index) { rel.index = index; }
};

#endif /* INCLUDE_INMEMORYRELATIONTEST_H_ */
