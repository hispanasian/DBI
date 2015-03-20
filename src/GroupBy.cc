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
	Record &rec, Record &prev, ComparisonEngine &comp) {
	int intSum = 0, intResult = 0;
	double doubleSum = 0.0, doubleResutl = 0.0;

	if(sorted.Remove(&rec)) {
		prev.Copy(&rec);
	}

	outPipe.ShutDown();
}

int GroupBy::GetPageLimit() { return pageLimit; }
