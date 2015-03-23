#include "SortMergeJoiner.h"
#include "SortMergeJoinerTest.h"

PartialSortMergeJoiner::PartialSortMergeJoiner() {}

PartialSortMergeJoiner::~PartialSortMergeJoiner() {}

bool PartialSortMergeJoiner::StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimt, OrderMaker& orderL, ComparisonEngine& comp) {
	SortMergeJoiner::StreamLeftGroup(inPipeL, groupRecL, tempL, mergeInto, relL, relR, outPipe, memLimt, orderL, comp);
}
