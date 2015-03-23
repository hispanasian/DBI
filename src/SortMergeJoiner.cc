#include "SortMergeJoiner.h"

SortMergeJoiner::SortMergeJoiner() {}

SortMergeJoiner::~SortMergeJoiner() {}

bool SortMergeJoiner::AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
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

bool SortMergeJoiner::InitRightGroup(Pipe& inPipeR, Record& groupRec, Record& tempR, JoinRelation& relR,
		OrderMaker& orderR, ComparisonEngine& comp) {
	// relR.Add(&groupRec);
	while(true) {
		if(inPipeR.Remove(&tempR) == 0) { // the pipe is empty
			return true;
		} else { // we read a record
			if(comp.Compare(&groupRec, &tempR, &orderR) == 0) { // this record is part of the same group
				relR.Add(&tempR);
			} else { // we've reached the end of this group
					 // and we've set tempR to the first record
					 // from the next group
				return false;
			}
		}
	}
}

bool SortMergeJoiner::StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp) {
	relL.SetMemLimit(memLimit - relR.MemUsed());

	while(true) {
		if(inPipeL.Remove(&tempL) == 0) { // if the pipe is empty
			// merge what we have
			MergeRelations(relL, relR, outPipe, mergeInto);
			return true;
		} else if(comp.Compare(&groupRecL, &tempL, &orderL) == 0) { // this record is part of the group
			if(relL.Add(&tempL)) {
				// we added it successfully
			} else {
				// we're out of space, time to merge
				MergeRelations(relL, relR, outPipe, mergeInto);
				// make some room
				relL.Clear();
				relL.Add(&tempL);
			}
		} else { // this record is part of the next group
			MergeRelations(relL, relR, outPipe, mergeInto);
			return false;
		}
	}
}

void SortMergeJoiner::MergeRelations(InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, Record& mergeInto) {
	relL.Reset();
	relR.Reset();
	// It's important that the pointers we use to get the Records or not NULL, but can be memory managed
	Record tempL, tempR;
	Record* recL = &tempL;
	Record* recR = &tempR;

	// First, Get the first Records and create the merge data
	relL.GetNext(recL);
	relR.GetNext(recR);
	int numAttsLeft = recL->NumAtts();
	int numAttsRight = recR->NumAtts();
	int numAttsToKeep = numAttsLeft + numAttsRight;
	int startOfRight = numAttsLeft;
	int attsToKeep[numAttsLeft + numAttsRight];

	for(int i = 0; i < numAttsLeft; i++) { attsToKeep[i] = i; }
	for(int i = 0; i < numAttsRight; i++) { attsToKeep[i + numAttsLeft] = i; }

	// Outer loop on right and inner loop on left
	// because if right is a file, we only want to read it once
	do {
		do {
			mergeInto.MergeRecords(recL, recR, numAttsLeft, numAttsRight,
				attsToKeep, numAttsToKeep, startOfRight);
			outPipe.Insert(&mergeInto);		
		} while(relL.GetNext(recL));
		relL.Reset(); // Rest R so we can go through it again
		relL.GetNext(recL); // Get the next Record again
	} while(relR.GetNext(recR));
}

void SortMergeJoiner::Exit(Pipe& inPipeL, Pipe& inPipeR, Pipe& outPipe) {
	Record rec;
	while(inPipeL.Remove(&rec)) {
		// do nothing
	}
	while(inPipeR.Remove(&rec)) {
		// do nothing
	}
	outPipe.ShutDown();
}

void SortMergeJoiner::Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, 
	OrderMaker &orderL, OrderMaker &orderR, int memLimit) {
	
	Record nextL;
	Record nextR;

	// check if either pipe is empty and
	// initialize our cursors
	if(inPipeL.Remove(&nextL) == 0) {
		return Exit(inPipeL, inPipeR, outPipe);
	}

	if(inPipeR.Remove(&nextR) == 0) {
		return Exit(inPipeL, inPipeR, outPipe);
	}

	InMemoryRelation relL;
	JoinRelation relR = JoinRelation(memLimit);
	ComparisonEngine comp;

	bool leftDone = false;
	bool rightDone = false;
	while(true) {
		if(AlignGroups(inPipeL, inPipeR, nextL, nextR, orderL, orderR, comp)) {
			// no matching groups were found, no need to merge
			return Exit(inPipeL, inPipeR, outPipe);
		}

		Record copyR;
		Record tempR;
		copyR.Copy(&nextR);
		relR.Add(&copyR);
		if(InitRightGroup(inPipeR, nextR, tempR, relR, orderR, comp)) {
			rightDone = true;
		}

		Record copyL;
		Record tempL;
		Record mergeInto;
		copyL.Copy(&nextL);
		relL.Add(&copyL);
		if(StreamLeftGroup(inPipeL, nextL, tempL, mergeInto,
			relL, relR, outPipe, memLimit, orderL, comp)) {
			leftDone = true;
		}

		if(leftDone || rightDone) {
			return Exit(inPipeL, inPipeR, outPipe);
		}

		// clear relL and relR
		relL.Clear();
		relR.Clear();
		// consume tempR/L to nextR/L
		nextL.Consume(&tempL);
		nextR.Consume(&tempR);
	}
}