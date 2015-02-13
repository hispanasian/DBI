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

TPMMSContainer::TPMMSContainer(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp, OrderMaker &_order, vector<int> &_runPos, vector<Record *> &_run, int &_runlen):
	in(_in), out(_out), file(_file), page(_page), rec(new Record()), comp(_comp), order(_order), runPos(_runPos), run(_run), runlen(_runlen) {
}

TPMMSContainer::TPMMSContainer(Pipe &_in, Pipe &_out, OrderMaker &_sortorder, int &_runlen):
		in(_in), out(_out), file(myFile), page(myPage), rec(new Record()), comp(myComp), order(myOrder), runPos(myRunPos), run(myRun), runlen(_runlen) {

}

void TPMMSContainer::SortRun() {

}

void TPMMSContainer::AddRecord() {

}

int TPMMSContainer::Phase1() {
	return 0;
}

void TPMMSContainer::Phase2() {

}

 void TPMMS() {

 }
