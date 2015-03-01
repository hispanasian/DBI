#include "BigQ.h"
#include "Defs.h"
#include "../include/PipedPage.h"
#include <algorithm>
#include <iostream>
#include <cstdio>

BigQ :: BigQ () {}

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
    TPMMS* tpmms = new TPMMS(in, out, sortorder, runlen);
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
		in(in), out(out), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(order), runPos(myRunPos), run(myRun), runlen(0) {
	totalPageCount = 0;
	runSizeInBytes = PAGE_SIZE * runlen;
	currRunSizeInBytes = 0;
}

TPMMS::TPMMS(Pipe &_in, Pipe &_out, OrderMaker &_sortorder, int &_runlen):
		in(_in), out(_out), file(myFile), page(myPage), rec(new Record()), comp(myComp),
		order(_sortorder), runPos(myRunPos), run(myRun), runlen(_runlen) {
	totalPageCount = 0;
	runSizeInBytes = PAGE_SIZE * runlen;
	currRunSizeInBytes = 0;
}

TPMMS::~TPMMS() {
	// delete rec;
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

bool TPMMS::AddRecord(Record* &rec) {
	if(rec->Size()+currRunSizeInBytes <= runSizeInBytes) {
		run.push_back(rec);
		currRunSizeInBytes += rec->Size();
		rec = new Record(); // can't re-use object
		return true;
	} else if(run.size() == 0) {
		throw std::runtime_error("rec exceeds the Page size");
	}
	return false;
}

void TPMMS::Phase1() {
	totalPageCount = 0;
	runPos.push_back(totalPageCount);
	currRunSizeInBytes = runlen * sizeof (int);
	while(in.Remove(rec)) {
		if(!AddRecord(rec)) {
			SortRun();
			RunToFile(totalPageCount);
			runPos.push_back(totalPageCount);
			currRunSizeInBytes = runlen * sizeof (int); // Reset currRunsSizeInButes
			AddRecord(rec); // Add the record that failed to get added
		}
	}
	if(run.size() != 0) {
		SortRun();
		RunToFile(totalPageCount);
		runPos.push_back(totalPageCount);
	}
}

void TPMMS::GetNextRecord(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft) {
	if(!pages[min]->GetFirst(heads[min])) {
		// We have run out of pages
		++runIndex[min];
		if(runIndex[min] < runPos[min+1]) {
			file.GetPage(pages[min], runIndex[min]);
			pages[min]->GetFirst(heads[min]); // Get the missing recordR
		}
		else{
			 --runsLeft; // The run has run out of pages.
			 delete heads[min];
			 heads[min] = NULL;
		}

	}
}

int TPMMS::FindMin(int size, Record **&heads) {
	int minIndex = -1;
	Record *min = NULL;

	for(int i = 0; i < size; i++) {
		if(heads[i] != NULL && ( min == NULL || comp.Compare(min, heads[i], &order) > 0 )) {
			min = heads[i];
			minIndex = i;
		}
	}
	return minIndex;
}

void TPMMS::Phase2() {
	int minIndex = -1;
	int totalRuns = runPos.size() - 1;
	int runsLeft = totalRuns;

	// initialize
	Record **heads = new Record*[totalRuns];
	off_t *runIndex = new off_t[totalRuns];
	Page ** pages = new Page*[totalRuns];

	for(int i = 0; i < totalRuns; i++) {
		heads[i] = new Record();
		runIndex[i] = runPos[i];
		pages[i] = new Page();
		file.GetPage(pages[i], runIndex[i]);
		pages[i]->GetFirst(heads[i]);
	}

	while(runsLeft > 0) {
		minIndex = FindMin(totalRuns, heads);
		out.Insert(heads[minIndex]);
		GetNextRecord(minIndex, heads, runIndex, pages, runsLeft);
	}

	// Clean up
	for(int i = 0; i < totalRuns; i++) {
		delete heads[i];
		delete pages[i];
	}
	delete []heads;
	delete []runIndex;
	delete []pages;
}

void TPMMS::Sort() {
	char* fname = tmpnam(NULL);
	while(fname == NULL) { fname = tmpnam(NULL); } // a temp file name
	file.Open(0, fname);

	Phase1();
	Phase2();

	// Clean up
	out.ShutDown();
	file.Close();
	remove(fname);
	delete rec;
	runPos.clear();
}

void TPMMS::Merge(PipedPage *p1, PipedPage *p2) {
	int minIndex = -1;
	int totalRuns = 2;
	int runsLeft = totalRuns;

	// initialize
	Record **heads = new Record*[totalRuns];
	off_t *runIndex = new off_t[totalRuns];
	Page **pages = new Page*[totalRuns];

	heads[0] = new Record();
	heads[1] = new Record();
	runIndex[0] = 0;
	runIndex[1] = 1;
	pages[0] = p1;
	pages[1] = p2;

	// Check to make sure the pages are full
	if(pages[0]->GetFirst(heads[0]) == 0) {
		--runsLeft;
		heads[0] = NULL;
	}
	if(pages[1]->GetFirst(heads[1]) == 0) {
		--runsLeft;
		heads[1] = NULL;
	}
	runPos.clear();
	runPos.push_back(0);
	runPos.push_back(0);
	runPos.push_back(1);

	while(runsLeft > 0) {
		minIndex = FindMin(totalRuns, heads);
		out.Insert(heads[minIndex]);
		GetNextRecord(minIndex, heads, runIndex, pages, runsLeft);
	}

	// Clean up
	out.ShutDown();
	// for(int i = 0; i < totalRuns; i++) {
	// 	delete heads[i];
	// }
	cout << "Deleting stuff in Merge" << endl;
	delete heads[0];
	delete heads[1];
	delete []heads;
	delete []runIndex;
	delete []pages;
	delete rec;
}
