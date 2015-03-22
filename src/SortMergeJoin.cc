#include "SortMergeJoin.h"

SortMergeJoin::SortMergeJoin() {}

SortMergeJoin::~SortMergeJoin() {}

bool SortMergeJoin::AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp) { 
	while(true) {
		// check if the recs are in the same group
		int compVal = comp.Compare(&tempL, &orderL, &tempR, &orderR);
		// they're in the same group
		if(compVal == 0) {
			return false;
		} else if(compVal < 0) { // left < right
			// try to advance the left 
			if(inPipeL.Remove(&tempL) == 0) { // left pipe is empty
				return true;
			}
		} else { // left > right
			// try to advance the right
			if(inPipeR.Remove(&tempR) == 0) { // right pipe is empty
				return true;
			}
		}
	}
}

void SortMergeJoin::Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR) {}