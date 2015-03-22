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

class PartialSortMergeJoin: public SortMergeJoin {
public:
	// virtual void MergeRelations(InMemoryRelation relL, JoinRelation relR, Pipe& outPipe);
	MOCK_METHOD4(MergeRelations, void(InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, Record& rec));
	PartialSortMergeJoin();
	virtual ~PartialSortMergeJoin();
	bool StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp);
};

class SortMergeJoinTest: public ::testing::Test {
public:
	SortMergeJoin join;
	PartialSortMergeJoin mock;
	StrictMock<MockPipe> inL;
	StrictMock<MockPipe> inR;
	StrictMock<MockPipe> out;
	StrictMock<MockCNF> cnf;
	StrictMock<MockRecord> tempL;
	StrictMock<MockRecord> tempR;
	StrictMock<MockRecord> groupRecL;
	StrictMock<MockRecord> groupRecR;
	StrictMock<MockRecord> mergeInto;
	StrictMock<MockOrderMaker> orderL;
	StrictMock<MockOrderMaker> orderR;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockJoinRelation> jrel;
	StrictMock<MockInMemoryRelation> relL;
	StrictMock<MockJoinRelation> relR;
	StrictMock<MockInMemoryRelation> imrel;
	int memLimit = 1000;

	bool AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
			OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp) {
		return join.AlignGroups(inPipeL, inPipeR, tempL, tempR, orderR, orderL, comp);
	}

	bool InitRightGroup(Pipe& inPipeR, Record& groupRecR, Record& tempR, JoinRelation& relR,
		OrderMaker& orderR, ComparisonEngine& comp) {
		return join.InitRightGroup(inPipeR, groupRecR, tempR, relR, orderR, comp);
	}

	bool StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp) {
		return mock.StreamLeftGroup(inPipeL, groupRecL, tempL, mergeInto, relL, relR, outPipe, memLimit, orderL, comp);
	}

};

#endif