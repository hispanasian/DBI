/*
 * DuplicateRemoval.cc
 *
 *  Created on: Mar 13, 2015
 *      Author: cvasquez
 */

#include "DuplicateRemoval.h"

DuplicateRemoval::DuplicateRemoval() {
	pageLimit = 1;
}

DuplicateRemoval::~DuplicateRemoval() {/* Do nothing */}

void DuplicateRemoval::Use_n_Pages (int n) {

}

void DuplicateRemoval::Run(Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {

}

void DuplicateRemoval::Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {

}

void DuplicateRemoval::Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, ComparisonEngine &comp, OrderMaker &order) {

}

int DuplicateRemoval::GetPageLimit() {
	return 0;
}
