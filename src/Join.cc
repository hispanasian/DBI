/*
 * Join.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "Join.h"
#include "BigQ.h"

Join::Join() {
	memLimit = PAGE_SIZE;
	pageLimit = 1;
}

Join::~Join() {
	// TODO Auto-generated destructor stub
}

void Join::Use_n_Pages (int n) {
	pageLimit = n;
	memLimit = pageLimit * PAGE_SIZE;
}

void Join::Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal) {
	JoinData *data = new JoinData { inPipeL, inPipeR, outPipe, selOp, literal, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		JoinData *data = (JoinData*)args;
		data->op.Work(data->inL, data->inR, data->out, data->selOp, data->literal);
		delete data;
	}, (void*) data);
	if(thread_id) {
    	cout << "Unable to create thread in Join: " << thread_id << endl;
    }
}

void Join::Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal) {
	ComparisonEngine comp;
	OrderMaker orderL, orderR;
	Work(inPipeL, inPipeR, outPipe, selOp, literal, comp, orderL, orderR);
}

void Join::Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal,
		ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR) {

	// We can do a sort-merge join
	if(selOp.GetSortOrders(orderL, orderR)) SortMergeJoin(inPipeL, inPipeR, outPipe, orderL, orderR);
		
	// We have to do a block nested loop join
	else BlockNestedLoopJoin(inPipeL, inPipeR, outPipe, selOp, literal);
}

void Join::SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR) {
	Pipe sortedLeft;
	BigQ left = BigQ(pipeL, sortedLeft, orderL, (pageLimit+1) / 2);
	Pipe sortedRight;
	BigQ right = BigQ(pipeR, sortedRight, orderR, (pageLimit+1) / 2);	
	SortMergeJoiner smj;
	smj.Join(sortedLeft, sortedRight, outPipe, orderL, orderR, memLimit);
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
