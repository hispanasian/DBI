#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "File.h"
#include "Record.h"

using namespace std;

class BigQ {

public:

	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();
};

class TPMMS {
friend class TPMMSTest;
friend class MockTPMMS;
private:
	Pipe &in;
	Pipe &out;
	File &file;
	File myFile;
	Page &page;
	Page myPage;
	Record *rec;
	ComparisonEngine &comp;
	ComparisonEngine myComp;
	OrderMaker &order;
	OrderMaker myOrder;
	vector<int> &runPos;
	vector<int> myRunPos;
	vector<Record *> &run;
	vector<Record *> myRun;
	int runlen;

	TPMMS();
	TPMMS(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp,
			OrderMaker &_order, vector<int> &_runPos, vector<Record *> &_run, int &runlen);

	/**
	 * This function will run an in place sort on run using comp based on the order provided.
	 */
	virtual void SortRun();

	/**
	 * Moves the records from page to run.
	 */
	virtual void PageToRun();

	/**
	 *	Adds record to the run. If record fills the current page, the page will be written to disk
	 *	and modify any dependent objects (ie counters).
	 */
	virtual void AddRecord();

	/**
	 * Phase 1 of the TPMMS algorithm. This algorithm will take records from in and, once runlen
	 * number of pages have been filled (or there are no more pages), sort the records and write
	 * them to file. It will repeat this process until there are no more records (in has been shut
	 * down). This will return the number of runs that have been generated.
	 */
	virtual int Phase1();

	/**
	 * Phase 2 of the TPMMS algorithm. This phase will take the runs from disk and merge them into
	 * sorted pages that will be put into out.
	 */
	virtual void Phase2();

public:
	TPMMS(Pipe &in, Pipe &out, OrderMaker &sortorder, int &runlen);
	virtual ~TPMMS();

	/**
	 * Begin the process of reading input from in, sorting the it, and sending the sorted Records
	 * to out.
	 */
	virtual void Sort();
};

/**
 * Two-Phase Multiway merge Sort
 */
void Work(void *ptr);

#endif
