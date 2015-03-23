/*
 * Sum.cc
 *
 *  Created on: Mar 18, 2015
 *      Author: cvasquez
 */

#include "Sum.h"

Sum::Sum() {/* Do nothing */}

Sum::~Sum() {/* Do nothing */}

void Sum::Use_n_Pages(int n) {/* Do nothing */}

void Sum::Run(Pipe &inPipe, Pipe &outPipe, Function &computeMe) {
	SumData *data = new SumData { inPipe, outPipe, computeMe, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		SumData *data = (SumData*)args;
		data->op.Work(data->in, data->out, data->computeMe);
		delete data;
	}, (void*) data);
}

void Sum::Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe) {
	Record rec;
	Work(inPipe, outPipe, computeMe, rec);
}

void Sum::Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec) {
	int intSum = 0;
	double doubleSum = 0;

	int intResult = 0;
	double doubleResult = 0;

	while(inPipe.Remove(&rec)) {
		computeMe.Apply(rec, intResult, doubleResult);
		intSum += intResult;
		doubleSum += doubleResult;
	}

	if(computeMe.ReturnsInt()) {
		Record sum = Record(intSum);
		outPipe.Insert(&sum);
	}
	else {
		Record sum = Record(doubleSum);
		outPipe.Insert(&sum);
	}

	outPipe.ShutDown();
}
