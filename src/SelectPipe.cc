/*
 * SelectPipe.cpp
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#include "../include/SelectPipe.h"

SelectPipe::SelectPipe() {/* Do nothing */}

SelectPipe::~SelectPipe() {/* Do nothing */}

void SelectPipe::Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) {
	SelectPipeData *data = new SelectPipeData { inPipe, outPipe, selOp, literal, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		SelectPipeData *data = (SelectPipeData*)args;
		data->op.Select(data->in, data->out, data->selOp, data->literal);
		delete data;
	}, (void*) data);
	if(thread_id) {
    	cout << "Unable to create thread in SelectPipe: " << thread_id << endl;
    }
}

void SelectPipe::Use_n_Pages (int n) {/* Do nothing */}

void SelectPipe::Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) {
	ComparisonEngine comp;
	Record rec;
	Select(inPipe, outPipe, selOp, literal, comp, rec);
}

void SelectPipe::Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec) {
	while(inPipe.Remove(&rec)) {
		if(comp.Compare(&rec, &literal, &selOp)) outPipe.Insert(&rec);
	}
	// Important, must shut down outPipe
	outPipe.ShutDown();
}
