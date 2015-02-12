#include "BigQ.h"
#include "Defs.h"
#include <algorithm>
#include <iostream>

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
    TPMMS* tpmms = new TPMMS(in, out, sortorder, runlen);
    cout << "Creating thread" << endl;
    pthread_t worker;
    int ret = pthread_create(&worker, NULL, Work, (void*) tpmms);
    if(ret) {
    	cout << "Unable to create thread " << ret << endl;
    }
}

BigQ::~BigQ () {
}

void* Work(void *ptr) {
	TPMMS *tpmms = (TPMMS*) ptr;
	tpmms->Sort();
	delete tpmms;
	pthread_exit(NULL);
}

TPMMS::TPMMS(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp,
		OrderMaker &_order, vector<off_t> &_runPos, vector<Record *> &_run, int &_runlen):
	in(_in), out(_out), file(_file), page(_page), rec(new Record()), comp(_comp), order(_order),
	runPos(_runPos), run(_run), runlen(_runlen) {
	totalPageCount = 0;
	runSizeInBytes = PAGE_SIZE * runlen;
	currRunSizeInBytes = 0;
}

TPMMS::TPMMS():
		in(myIn), out(myOut), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(myOrder), runPos(myRunPos), run(myRun), runlen(0) {
	totalPageCount = 0;
	runSizeInBytes = PAGE_SIZE * runlen;
	currRunSizeInBytes = 0;
}

TPMMS::TPMMS(Pipe &_in, Pipe &_out, OrderMaker &_sortorder, int &_runlen):
		in(_in), out(_out), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(myOrder), runPos(myRunPos), run(myRun), runlen(_runlen) {
	totalPageCount = 0;
	runSizeInBytes = PAGE_SIZE * runlen;
	currRunSizeInBytes = 0;
}

TPMMS::~TPMMS() {
}

void TPMMS::SortRun() {
	std::sort(run.begin(), run.end(), [&] (Record *left, Record *right) {
		return (comp.Compare(left, right, &order) < 0); // if both are equal, return right.
	} );
}

void TPMMS::RunToFile(off_t &totalPageCount) {
	page.EmptyItOut();
	for(vector<Record*>::iterator it = run.begin(); it != run.end(); it++) {
		if(!page.Append(*it)) {
			// Page full
			file.AddPage(&page, totalPageCount);
			++totalPageCount;
			page.EmptyItOut();
			// Now add the record that could not be added
			if(!page.Append(*it)) throw std::runtime_error("A record exceeds the Page size");
		}
	}

	// Write out the last page
	file.AddPage(&page, totalPageCount);
	++totalPageCount;
	page.EmptyItOut();

	// Delete Records and clear run
	for_each(run.begin(), run.end(),[] (Record *&rec) {
		delete rec;
		return true;
	});
	run.clear();
}

bool TPMMS::AddRecord(Record* rec) {
	if(rec->Size()+currRunSizeInBytes <= runSizeInBytes) {
		run.push_back(rec);
		currRunSizeInBytes += rec->Size();
		return true;
	} else if(run.size() == 0) {
		throw std::runtime_error("rec exceeds the Page size");
	}
	return false;
}

void TPMMS::Phase1() {
	while(in.Remove(rec)) {
		if(!AddRecord(rec)) {
			SortRun();
			RunToFile(totalPageCount);
			runPos.push_back(totalPageCount);
			AddRecord(rec); // Add the record that failed to get added
		}
	}
	if(run.size() != 0) {
		SortRun();
		RunToFile(totalPageCount);
		runPos.push_back(totalPageCount);
	}
}

void TPMMS::GetNextRecord(int min, Record *head[]) {

}

int TPMMS::FindMin(Record *head[]) {
	return 0;
}

void TPMMS::Phase2() {


	file.Close();
	remove("sortingtemp.bin");
	out.ShutDown();
}

void TPMMS::Sort() {
	std::cout << "Sorting time!" << std::endl;
	//file.Create(0, "sortingtemp.bin"); // TODO: make actually random name.
	// use http://www.cplusplus.com/reference/cstdio/tmpnam/ for rand file names
}
