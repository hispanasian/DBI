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

int Phase1(Pipe &in, Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, const int &runlen) {
	return 0;
}

int Phase1(Pipe &in, Pipe &out, OrderMaker &order, ComparisonEngine &comp, File &file,
		vector<int> &runPos,  Page &page, const int &runlen) {
	return 0;
}

void Phase2(Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, const int &runcount) {

}

void Phase2(Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, vector<Page*> &runs, const int &runcount) {

}
