#include "GroupBy.h"
#include "BigQ.h"

GroupBy::GroupBy() {
	pageLimit = 1;
}

GroupBy::~GroupBy() {}

void GroupBy::Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe) {}

void GroupBy::Use_n_Pages (int n) { pageLimit = n; }

void GroupBy::Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe) {}

void GroupBy::Work (Pipe &sorted, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe,
	Record &rec, Record &prev, Record& mergeInto, ComparisonEngine &comp) {
	int intSum = 0, intResult = 0;
	double doubleSum = 0.0, doubleResult = 0.0;

	if(sorted.Remove(&rec)) {
		prev.Copy(&rec);
	} else { // there are no records in the input pipe, we're done
		// close the output pipe
		outPipe.ShutDown();
		return;
	}

	// compute sum for the first record
	computeMe.Apply(prev, intResult, doubleResult);
	intSum += intResult; doubleSum += doubleResult;

	int* attsToKeep = new int[groupAtts.GetNumAtts() + 1];
    attsToKeep[0] = 0;
    for(int i = 0; i < groupAtts.GetNumAtts(); i++) {
    	attsToKeep[i+1] = groupAtts.GetAttIndex(i);
    }

	while(sorted.Remove(&rec)) {
		if(comp.Compare(&prev, &rec, &groupAtts) != 0) {
			// We've scanned through an entire group
			computeAndOutputSum(intSum, doubleSum, computeMe, prev, mergeInto,
				groupAtts, attsToKeep, outPipe);

			// reset our sums
			intSum = 0, intResult = 0;
			doubleSum = 0.0, doubleResult = 0.0;
		}
		computeMe.Apply(rec, intResult, doubleResult);
		intSum += intResult;
		doubleSum += doubleResult;
		prev.Copy(&rec);
	}

	// compute the result for the last group
	computeAndOutputSum(intSum, doubleSum, computeMe, prev, mergeInto,
			groupAtts, attsToKeep, outPipe);

	delete[] attsToKeep;
	// close the output pipe
	outPipe.ShutDown();
}

void GroupBy::computeAndOutputSum(int intSum, double doubleSum, Function& func, Record& mergeWith,
	Record& mergeInto, OrderMaker &groupAtts, int* attsToKeep, Pipe& outPipe) {
	if(func.ReturnsInt()) {
		Record sum = Record(intSum);
		mergeInto.MergeRecords(&sum, &mergeWith, 1, groupAtts.GetNumAtts(),
			attsToKeep, 1 + groupAtts.GetNumAtts(), 1);
		outPipe.Insert(&mergeInto);
	} else {
		Record sum = Record(doubleSum);
		mergeInto.MergeRecords(&sum, &mergeWith, 1, groupAtts.GetNumAtts(),
			attsToKeep, 1 + groupAtts.GetNumAtts(), 1);
		outPipe.Insert(&mergeInto);
	}
}

int GroupBy::GetPageLimit() { return pageLimit; }
