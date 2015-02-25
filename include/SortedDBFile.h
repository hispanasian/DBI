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

#ifndef INCLUDE_SORTEDDBFILE_H_
#define INCLUDE_SORTEDDBFILE_H_

#define PIPE_SIZE 100

struct SortInfo {
	OrderMaker *myOrder;
	int runLength;
};

struct MergeData {
	PipedPage *p1;
	PipedPage *p2;
	TPMMS *tpmms;
};

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
	char *f_path;
	bool recordAdded;
	Pipe *in;
	Pipe *out; // The Pipe used to get the added Records

	/**
	 * This is a function will be called by the public Load and it will provide it's own Record.
	 * The Load logic will be put in here. The existence of this function is purely for testing
	 * purposes (to Mock record).
	 */
	virtual void Load (Schema &myschema, char *loadpath, Record &record);

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

	SortedDBFile();

public:
	SortedDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *f_path, SortInfo *sortInfo);
    virtual ~SortedDBFile();

    /**
	 * Appends the data located at loadpath to memory.
	 * @param myschema	The schema of the file being loaded to memory
	 * @param loadpath	The path to the file being loaded
	 */
	virtual void Load (Schema &myschema, char *loadpath);

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
