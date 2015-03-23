#include "SortMergeJoin.h"
#include "SortMergeJoinTest.h"

PartialSortMergeJoin::PartialSortMergeJoin() {}

PartialSortMergeJoin::~PartialSortMergeJoin() {}

bool PartialSortMergeJoin::StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimt, OrderMaker& orderL, ComparisonEngine& comp) {
	SortMergeJoin::StreamLeftGroup(inPipeL, groupRecL, tempL, mergeInto, relL, relR, outPipe, memLimt, orderL, comp);
}
