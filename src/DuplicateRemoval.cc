/*
 * DuplicateRemoval.cc
 *
 *  Created on: Mar 13, 2015
 *      Author: cvasquez
 */

#include "DuplicateRemoval.h"
#include "BigQ.h"

DuplicateRemoval::DuplicateRemoval() {
	pageLimit = 1;
}

DuplicateRemoval::~DuplicateRemoval() {/* Do nothing */}

void DuplicateRemoval::Use_n_Pages (int n) {
	this->pageLimit = n;
}

void DuplicateRemoval::Run(Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
	DuplicateRemovalData *data = new DuplicateRemovalData { inPipe, outPipe, mySchema, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		DuplicateRemovalData *data = (DuplicateRemovalData*)args;

		// Sort the input data
		Pipe sortedRecs;
		OrderMaker order = OrderMaker(&(data->schema));
		BigQ q = BigQ(data->in, sortedRecs, order, data->op.GetPageLimit());

		data->op.Remove(sortedRecs, data->out, data->schema);
		delete data;
	}, (void*) data);
}

void DuplicateRemoval::Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
	Record rec;
	Record prev;
	ComparisonEngine comp;
	OrderMaker order = OrderMaker(&mySchema);
	Remove(inPipe, outPipe, mySchema, rec, prev, comp, order);
}

void DuplicateRemoval::Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order) {
	// The initial Record
	if(inPipe.Remove(&rec)) {
		prev.Copy(&rec);
		outPipe.Insert(&rec);

		while(inPipe.Remove(&rec)) {
			if(comp.Compare(&prev, &rec, &order) != 0) {
				prev.Copy(&rec);
				outPipe.Insert(&rec);
			}
		}
	}
	// Important, must shut down outPipe
	outPipe.ShutDown();
}

int DuplicateRemoval::GetPageLimit() {
	return pageLimit;
}
