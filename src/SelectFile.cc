/*
 * SelectFile.cc
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#include "../include/SelectFile.h"

SelectFile::SelectFile() {/* Do nothing */}

SelectFile::~SelectFile() {/* Do nothing */}


void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	SelectFileData *data = new SelectFileData { inFile, outPipe, selOp, literal, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		SelectFileData *data = (SelectFileData*)args;
		data->op.Select(data->in, data->out, data->selOp, data->literal);
		delete data;
	}, (void*) data);
}

void SelectFile::Use_n_Pages (int n) {/* Do nothing */}

void SelectFile::Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	ComparisonEngine comp;
	Record rec;
	Select(inFile, outPipe, selOp, literal, comp, rec);
}

void SelectFile::Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec) {
	while(inFile.GetNext(rec)) {
		if(comp.Compare(&rec, &literal, &selOp)) outPipe.Insert(&rec);
	}
	// Important, must shut down outPipe
	outPipe.ShutDown();
}
