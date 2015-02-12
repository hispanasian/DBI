#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	// read data from in pipe sort them into runlen pages

    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

    // finally shut down the out pipe
	out.ShutDown ();
}

BigQ::~BigQ () {
}

TPMMS::TPMMS(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp,
		OrderMaker &_order, vector<int> &_runPos, vector<Record *> &_run, int &_runlen):
	in(_in), out(_out), file(_file), page(_page), rec(new Record()), comp(_comp), order(_order),
	runPos(_runPos), run(_run), runlen(_runlen) {
}

TPMMS::TPMMS():
		in(in), out(out), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(myOrder), runPos(myRunPos), run(myRun), runlen(0) {

}

TPMMS::TPMMS(Pipe &_in, Pipe &_out, OrderMaker &_sortorder, int &_runlen):
		in(_in), out(_out), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(myOrder), runPos(myRunPos), run(myRun), runlen(_runlen) {

}

TPMMS::~TPMMS() {

}

void TPMMS::SortRun() {

}

void TPMMS::PageToRun() {

}

void TPMMS::AddRecord() {

}

int TPMMS::Phase1() {
	return 0;
}

void TPMMS::Phase2() {

}

void TPMMS::Sort() {

}
