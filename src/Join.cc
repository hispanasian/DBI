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
	if(selOp.GetSortOrders(orderL, orderR)) SortMergeJoin(inPipeL, inPipeR, outPipe, orderL, orderR);
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

