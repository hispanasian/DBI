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

struct TPPMSData {

};

/**
 * Two-Phase Multiway merge Sort
 */
void TPMMS();

/**
 * Phase 1 of the TPMMS algorithm. This algorithm will take records from in and, once runlen
 * number of pages have been filled (or there are no more pages), sort the records and write them
 * to file. It will repeat this process until there are no more records (in has been shut down).
 * This will return the number of runs that have been generated. Note: This particular Phase1
 * function contains the definition for Phase1, but this is meant for testing. Please use the
 * Phase1 function with fewer arguments.
 * @param in		The input Pipe
 * @param out		The output Pipe
 * @param order		The ordering definition
 * @param file		The file that will store the sorted Pages
 * @param runPos	A vector that will contain the starting page index of each run
 * @param runlen	The number of Pages in a run
 * @return			The number of runs that have been produced
 */
int Phase1(Pipe &in, Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, const int &runlen);

/**
 * Phase 1 of the TPMMS algorithm. This algorithm will take records from in and, once runlen
 * number of pages have been filled (or there are no more pages), sort the records and write them
 * to file. It will repeat this process until there are no more records (in has been shut down).
 * This will return the number of runs that have been generated. Note: This particular Phase1
 * function contains the definition for Phase1, but this is mean't for testing. Please use the
 * Phase1 function with fewer arguments.
 * @param in		The input Pipe
 * @param out		The output Pipe
 * @param order		The ordering definition
 * @param comp		The engine that will be used to compare Records
 * @param file		The file that will store the sorted Pages
 * @param runPos	A vector that will contain the starting page index of each run
 * @param psge		A page that will temporarily hold the records
 * @param runlen	The number of Pages in a run
 * @return			The number of runs that have been produced
 */
int Phase1(Pipe &in, Pipe &out, OrderMaker &order, ComparisonEngine &comp, File &file,
		vector<int> &runPos,  Page &page, const int &runlen);

/**
 * Phase 2 of the TPMMS algorithm. This phase will take the runs from disk and merge them into
 * sorted pages that will be put into out.
 * @param out		The output Pipe
 * @param order		The ordering definition
 * @param file		The file that will store the sorted Pages
 * @param runPos	A vector that will contain the starting page index of each run
 * @param runcount	The number of runs
 */
void Phase2(Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, const int &runcount);

/**
 * Phase 2 of the TPMMS algorithm. This phase will take the runs from disk and merge them into
 * sorted pages that will be put into out. This particular Phase2 function contains the definition
 * for Phase2, but this is meant for testing. Please use the Phase2 function with fewer arguments.
 * @param out		The output Pipe
 * @param order		The ordering definition
 * @param file		The file that will store the sorted Pages
 * @param runPos	A vector that will contain the starting page index of each run
 * @param runs		A vector that keeps the "smallest" pages of each run in memory.
 * @param runcount	The number of runs
 */
void Phase2(Pipe &out, OrderMaker &order, File &file, vector<int> &runPos, vector<Page*> &runs, const int &runcount);

#endif
