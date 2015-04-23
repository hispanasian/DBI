/*
 * SortedDBFile.h
 *
 *  Created on: Feb 16, 2015
 *      Author: cvasquez
 */

#include "GenericDBFile.h"
#include "Pipe.h"
#include "BigQ.h"
#include "HeapDBFile.h"
#include "LinearScanner.h"

#ifndef INCLUDE_SORTEDDBFILE_H_
#define INCLUDE_SORTEDDBFILE_H_

#define PIPE_SIZE 100

// struct SortInfo {
// 	OrderMaker *myOrder;
// 	int& runLength;
// };

class SortInfo {
public:
	OrderMaker *myOrder;
	int* runLength;
	SortInfo(OrderMaker* _myOrder, int* _runLength): myOrder(_myOrder), runLength(_runLength) {}
	~SortInfo() { delete myOrder; delete runLength; }
};

struct MergeData {
	PipedPage *p1;
	PipedPage *p2;
	TPMMS *tpmms;
};

enum RWState { Reading, Writing };
enum GetNextState { UseCNF, NoCNF };

/**
 *	SortedDBFile will store records in a sorted manner.
 *	Load and Add should always set the recordAdded flag to true adn the Close, Read, and MoveFirst
 *	methods should always call flush which should check if a record was added and flush added
 *	Records.
 */
class SortedDBFile: public GenericDBFile {
friend class SortedDBFileTest;
friend class MockSOrtedDBFile;
friend class PartialSortedDBFileMock;
private:
	SortInfo *sortInfo;
	const char *f_path;
	Pipe *in;
	Pipe *out; // The Pipe used to get the added Records
	RWState rwState; // flag to denote reading/writing mode
	GetNextState getNextState; // flag to denote if using previously provided CNF
	Page* cursor;
	off_t cursorIndex;
	LinearScanner *scanner;

	/**
	 * This is a function will be called by the public Load and it will provide it's own Record.
	 * The Load logic will be put in here. The existence of this function is purely for testing
	 * purposes (to Mock record).
	 */
	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

	/**
	 * Flushes/Merges the new Records with the current File using temp as the temporary. temp
	 * should be unopened
	 */
	virtual void Flush(File &temp);

	/**
	 * Flushes/Merges the new Records and the old records into the file used by temp. The file used
	 * by temp is expected to be Open. The real work will get done here.
	 */
	virtual void Flush(HeapDBFile &temp);

	/**
	 * Sets the cursor and cursorIndex in the first position (ie, the first GetFiist class will get
	 * the Record) where a Record is found to match literal and query.
	 * @param literal	The Record used to compare against
	 * @param query		The order being compared on
	 * @return			True if a match was found
	 */
	virtual bool BinarySearch(Record &literal, OrderMaker &query);

	/**
	 * Same thing as BinarySearch with two arguments but with the provided ComparisonEngine, Record,
	 * and Page.
	 */
	virtual bool BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page);

	/**
	 * Puts the requested Page into page. If index corresponds to the cursorIndex, puts a copy
	 * of cursor into page.
	 * @param page		The page that will hold the Page
	 * @param index		The index being looked for
	 */
	virtual void GetBSTPage(Page &page, off_t index);

	/**
	 * Looks for a valid record that "equals" the literal and query. Returns false if none can be
	 * found. This will only look at the page pointed to by index and the following Page. If such a
	 * page is found, this will set cursor and cursorIndex to the Page.
	 */
	virtual bool FindValidRecord(Record &literal, OrderMaker &query, off_t index);

	/**
	 * Note that all the Records and pages are assumed to be empty.
	 */
	virtual bool FindValidRecord(Record &literal, OrderMaker &query, off_t index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp);

	SortedDBFile();

public:
	SortedDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, const char *f_path, SortInfo *sortInfo);
    virtual ~SortedDBFile();

    /**
	 * Appends the data located at loadpath to memory.
	 * @param myschema	The schema of the file being loaded to memory
	 * @param loadpath	The path to the file being loaded
	 */
	virtual void Load (Schema &myschema, const char *loadpath);

	/**
	 * Moves the pointer from the current record to the first record of the first page.
	 */
	virtual void MoveFirst ();

	/**
	 * Adds addMe to the last page in memory. Writes are not immediately readable. In order to get
	 * the most immediate written records, you must call MoveFirst.
	 * @param addme	The Record being added to DBFile
	 */
	virtual void Add (Record &addme);

	/**
	 * Puts the current record in fetchme and moves the pointer to the next record. Upon use, the
	 * record referenced by fetchme is consumed and the new record in fetchme is removed from DB
	 * File.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme);

	/**
	 * Looks for the next record that passes applyMe when applied between a Record in DBFile and
	 * literal. This function increments the pointer accordingly.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @param applyMe	The CNF that will be applied between the Record(s) in DBFile and literal
	 * @param literal	The Record that will be compared with the Records in DBFile.
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);

	/**
	 * Writes out any changes to disk
	 */
	virtual void Flush();

	/**
	 * Initializes cursor and last.
	 */
	virtual void Initialize();

	/**
	 * Puts DBFile into an initial state.
	 */
	virtual void Reset();
};

#endif /* INCLUDE_SORTEDDBFILE_H_ */
