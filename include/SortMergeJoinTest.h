#ifndef SORTMERGEJOINTEST_H
#define SORTMERGEJOINTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "SortMergeJoin.h"

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
using ::testing::SetArgReferee;
using ::testing::DoAll;

class SortMergeJoinTest: public ::testing::Test {
public:
	SortMergeJoin join;
	StrictMock<MockPipe> inL;
	StrictMock<MockPipe> inR;
	StrictMock<MockPipe> out;
	StrictMock<MockCNF> cnf;
	StrictMock<MockRecord> tempL;
	StrictMock<MockRecord> tempR;
	StrictMock<MockRecord> groupRecR;
	StrictMock<MockOrderMaker> orderL;
	StrictMock<MockOrderMaker> orderR;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockJoinRelation> jrel;
	StrictMock<MockJoinRelation> relL;
	StrictMock<MockJoinRelation> relR;
	StrictMock<MockInMemoryRelation> imrel;

	bool AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
			OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp) {
		return join.AlignGroups(inPipeL, inPipeR, tempL, tempR, orderR, orderL, comp);
	}

	bool InitRightGroup(Pipe& inPipeR, Record& groupRecR, Record& tempR, JoinRelation& relR,
		OrderMaker& orderR, ComparisonEngine& comp) {
		return join.InitRightGroup(inPipeR, groupRecR, tempR, relR, orderR, comp);
	}

};

#endif