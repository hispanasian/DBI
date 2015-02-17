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
friend class MockForPhase1;
private:
	Pipe &in;
	Pipe myIn;
	Pipe &out;
	Pipe myOut;
	File &file;
	File myFile;
	Page &page;
	Page myPage;
	Record *rec;
	ComparisonEngine &comp;
	ComparisonEngine myComp;
	OrderMaker &order;
	OrderMaker myOrder;
	vector<off_t> &runPos;
	vector<off_t> myRunPos;
	vector<Record *> &run;
	vector<Record *> myRun;
	int currRunSizeInBytes;
	int runSizeInBytes;
	const int runlen;
	off_t totalPageCount;

	TPMMS();
	TPMMS(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp,
			OrderMaker &_order, vector<off_t> &_runPos, vector<Record *> &_run, int &runlen);

	/**
	 * This function will run an in place sort on run using comp based on the order provided.
	 */
	virtual void SortRun();

	/**
	 * Writes the run to file.
	 * @param totalPageSize	The current page size of the file. This will be incremented as new
	 * 						pages are added (ie, totalPageSize is modified).
	 */
	virtual void RunToFile(off_t &totalPageCount);

	/**
	 *	Adds rec to the page buffer. If record fills the page buffer, the page will be moved
	 *	to run. If this causes the number of pages to equal runlen, the record will not be added
	 *  and the method will return false. Otherwise it will return true.
	 */
	virtual bool AddRecord(Record* rec);

	/**
	 * Phase 1 of the TPMMS algorithm. This algorithm will take records from in and, once runlen
	 * number of pages have been filled (or there are no more pages), sort the records and write
	 * them to file. It will repeat this process until there are no more records (in has been shut
	 * down).
	 */
	virtual void Phase1();

	/**
	 * Replaces the old mininumum value located at min with a new value.
	 * @param min		The index of the minimum record
	 * @param heads		The array that will be updated with the new min value
	 * @param runIndex	The index of each run.
	 * @param runsLeft	The number of runs remaining
	 */
	virtual void GetNextRecord(int min, Record **&heads, off_t *&runIndex, int &runsLeft);

	/**
	 * Find the index of the minimum record in the heads.
	 * @param size		The size of heads
	 * @param heads		The head of each run.
	 * @return 			The index of the min Record in head
	 */
	virtual int FindMin(int size, Record **&heads);

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
void* Work(void *ptr);

#endif
