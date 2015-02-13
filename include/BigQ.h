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

class TPMMSContainer {
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

	TPMMSContainer(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp, OrderMaker &_order, vector<int> &_runPos, vector<Record *> &_run, int &runlen);
public:
	TPMMSContainer(Pipe &in, Pipe &out, OrderMaker &sortorder, int &runlen);
	~TPMMSContainer();

	/**
	 * This function will run an in place sort on run using comp based on the order provided.
	 */
	void SortRun();

	/**
	 * Moves the records from page to run.
	 */
	void PageToRun();

	/**
	 *	Adds record to the run. If record fills the current page, the page will be written to disk
	 *	and modify any dependent objects (ie counters).
	 */
	void AddRecord();

	/**
	 * Phase 1 of the TPMMS algorithm. This algorithm will take records from in and, once runlen
	 * number of pages have been filled (or there are no more pages), sort the records and write them
	 * to file. It will repeat this process until there are no more records (in has been shut down).
	 * This will return the number of runs that have been generated.
	 */
	int Phase1();

	/**
	 * Phase 2 of the TPMMS algorithm. This phase will take the runs from disk and merge them into
	 * sorted pages that will be put into out.
	 */
	void Phase2();
};

/**
 * Two-Phase Multiway merge Sort
 */
void TPMMS();

#endif
