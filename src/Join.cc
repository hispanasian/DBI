/*
 * Join.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "Join.h"

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

}

void Join::BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal,
		JoinRelation &relS, InMemoryRelation &relR, Record &rec) {

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
