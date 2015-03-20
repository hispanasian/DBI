#ifndef JOINTEST_H
#define JOINTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/Join.h"

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

class PartialJoinMock: public Join {
public:
//	virtual void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD5(Work, void(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal));
//	void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR);
	MOCK_METHOD5(SortMergeJoin, void(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR));
//	void BlocknestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD5(BlockNestedLoopJoin, void(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal));
	MOCK_METHOD5(Run, void(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal));
//	void MergeRelations(Relation &R, Relation &S, Record &rec);
	MOCK_METHOD3(MergeRelations, void(Relation &R, Relation &S, Record &rec));
//	void MergeRelations(Relation &R, Relation &S, Record &rec, ComparisonEngine &comp, Record &literal, CNF &cnf);
	MOCK_METHOD6(MergeRelations, void(Relation &R, Relation &S, Record &rec, ComparisonEngine &comp, Record &literal, CNF &cnf));

	PartialJoinMock();
	virtual ~PartialJoinMock();
	virtual void Use_n_Pages (int n);
//	virtual void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);
	void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR);
	void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR, JoinRelation &relR, JoinRelation &relS, Record &tempL, Record &tempR);
	void BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipR, Pipe &outPipe, CNF &selOp, Record &literal, JoinRelation &relS, InMemoryRelation &relR, Record &rec);
};

class JoinTest: public ::testing::Test {
public:
	PartialJoinMock op;
	Join join;
	StrictMock<MockPipe> inL;
	StrictMock<MockPipe> inR;
	StrictMock<MockPipe> out;
	StrictMock<MockCNF> cnf;
	StrictMock<MockRecord> literal;
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> tempL;
	StrictMock<MockRecord> tempR;
	StrictMock<MockOrderMaker> orderL;
	StrictMock<MockOrderMaker> orderR;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockJoinRelation> jrel;
	StrictMock<MockJoinRelation> relR;
	StrictMock<MockJoinRelation> relS;
	StrictMock<MockInMemoryRelation> imrel;

	void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR) {
		op.Work(inPipeL, inPipeR, outPipe, selOp, literal, comp, orderL, orderR);
	}
	void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR, JoinRelation &relR, JoinRelation &relS, Record &tempL, Record &tempR) {
		op.SortMergeJoin(pipeL, pipeR, outPipe, orderL, orderR, relR, relS, tempL, tempR);
	}
	void BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal, JoinRelation &relS, InMemoryRelation &relR, Record &rec) {
		op.BlockNestedLoopJoin(pipeL, pipeR, outPipe, selOp, literal, relS, relR, rec);
	}

	int GetMemLimit() { return op.memLimit; }
	void SetMemLimit(int lim) { op.memLimit = lim; }
	int GetPageLimit() { return op.pageLimit; }
	void SetPageLimit(int lim) { op.pageLimit = lim; }

	void MergeRelations(Relation &R, Relation &S, Record &rec) {
		join.MergeRelations(R, S, rec);
	}

	void MergeRelations(Relation &R, Relation &S, Record &rec, ComparisonEngine &comp, Record &literal, CNF &cnf) {
		join.MergeRelations(R, S, rec, comp, literal, cnf);
	}
};

#endif
