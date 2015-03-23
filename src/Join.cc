/*
 * Join.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "Join.h"
#include "BigQ.h"

Join::Join() {
	memLimit = 0;
	pageLimit = 0;
}

Join::~Join() {
	// TODO Auto-generated destructor stub
}

void Join::Use_n_Pages (int n) {

}

void Join::Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal) {

}

void Join::Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal) {
	ComparisonEngine comp;
	OrderMaker orderL, orderR;
	Work(inPipeL, inPipeR, outPipe, selOp, literal, comp, orderL, orderR);
}

void Join::Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal,
		ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR) {

	// We can do a sort-merge join
	if(selOp.GetSortOrders(orderL, orderR)) {
		Pipe sortedLeft;
		BigQ left = BigQ(inPipeL, sortedLeft, orderL, pageLimit);
		Pipe sortedRight;
		BigQ right = BigQ(inPipeR, sortedRight, orderR, pageLimit);
		SortMergeJoin(inPipeL, inPipeR, outPipe, orderL, orderR);	
	} 
	// We have to do a block nested loop join
	else BlockNestedLoopJoin(inPipeL, inPipeR, outPipe, selOp, literal);
}

void Join::SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL,
		OrderMaker &orderR) {

}

void Join::SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL,
		OrderMaker &orderR, JoinRelation &relR, JoinRelation &relS, Record &tempL, Record &tempR) {

}

void Join::BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal) {
	JoinRelation relS = JoinRelation(memLimit);
	InMemoryRelation relR;
	Record rec;
	BlockNestedLoopJoin(pipeL, pipeR, outPipe, selOp, literal, relS, relR, rec);
}

void Join::BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal,
		JoinRelation &relS, InMemoryRelation &relR, Record &rec) {
	Record *temp = &rec;
	ComparisonEngine comp;

	relS.Populate(pipeR);
	if(relS.MemUsed() > 0) { // There is work to be done
		relR.SetMemLimit(memLimit - relS.MemUsed());

		// Start getting Records from pipeL and put them into relR. Merge relR and relS when relR
		// becomes full
		while(pipeL.Remove(temp)) {
			if(!relR.Add(temp)) {
				MergeRelations(relR, relS, rec, outPipe, comp, literal, selOp);

				// Once the relations have been merged, clear relR and add the record that could
				// not be put in
				relR.Clear();
				relR.Add(temp);
			}
		}
		// Merge any Records that may have been left over
		if(relR.MemUsed() > 0) MergeRelations(relR, relS, rec, outPipe, comp, literal, selOp);
	}
	outPipe.ShutDown();
}

void Join::MergeRelations(Relation &R, Relation &S, Record &rec, Pipe &out) {

}

void Join::MergeRelations(Relation &R, Relation &S, Record &rec, Pipe &out, ComparisonEngine &comp, Record &literal, CNF &cnf) {
	// It's important that the pointers we use to get the Records or not NULL, but can be memory managed
	Record tempR, tempS;
	Record* recR = &tempR;
	Record *recS = &tempS;

	// First, Get the first Records and create the merge data
	R.GetNext(recR);
	S.GetNext(recS);
	int numAttsLeft = recR->NumAtts();
	int numAttsRight = recS->NumAtts();
	int numAttsToKeep = numAttsLeft + numAttsRight;
	int startOfRight = numAttsLeft;
	int attsToKeep[numAttsLeft + numAttsRight];

	for(int i = 0; i < numAttsLeft; i++) { attsToKeep[i] = i; }
	for(int i = 0; i < numAttsRight; i++) { attsToKeep[i + numAttsLeft] = i; }

	// Outer loop on S and inner loop on R
	do {
		do {
			if(comp.Compare(recR, recS, &literal, &cnf)) {
				rec.MergeRecords(recR, recS, numAttsLeft, numAttsRight, attsToKeep, numAttsToKeep, startOfRight);
				out.Insert(&rec);
			}
		} while(R.GetNext(recR));
		R.Reset(); // Rest R so we can go through it again
		R.GetNext(recR); // Get the next Record again
	} while(S.GetNext(recS));
}
