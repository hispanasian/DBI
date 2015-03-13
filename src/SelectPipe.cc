/*
 * SelectPipe.cpp
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#include "../include/SelectPipe.h"

SelectPipe::SelectPipe() {


}

SelectPipe::~SelectPipe() {

}

void SelectPipe::Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) {

}


void SelectPipe::WaitUntilDone () {/* Do nothing */}

void SelectPipe::Use_n_Pages (int n) {

}

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
