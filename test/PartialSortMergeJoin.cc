#include "SortMergeJoin.h"
#include "SortMergeJoinTest.h"

PartialSortMergeJoin::PartialSortMergeJoin() {}

PartialSortMergeJoin::~PartialSortMergeJoin() {}

bool PartialSortMergeJoin::StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimt, OrderMaker& orderL, ComparisonEngine& comp) {
	SortMergeJoin::StreamLeftGroup(inPipeL, groupRecL, tempL, relL, relR, outPipe, memLimt, orderL, comp);
}

// void PartialSortMergeJoin::Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
// 		Record &literal, OrderMaker &orderL, OrderMaker &orderR) {
// 	SortMergeJoin::Join(inPipeL, inPipeR, outPipe, selOp, literal, orderL, orderR);
// }