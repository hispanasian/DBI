#ifndef DBFILE_H
#define DBFILE_H

#include "HeapDBFile.h"
#include "GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "RawFile.h"
#include "DBConfig.h"
#include <sys/stat.h>

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
friend class DBFileTest;
friend class HeapDBFileTest;
friend class SortedDBFileTest;
private:
	File &file;
	File myFile;
	RawFile &rfile;
	RawFile myRFile;
	DBConfig &config;
	DBConfig myConfig;
	ComparisonEngine &comp;
	ComparisonEngine myComp;
	GenericDBFile *delegate;

	DBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp); // Strictly for Testing.

	/**
	 * Puts DBFile into an initial state.
	 */
	virtual void Reset();
	/*
	 * Initializes the DBFile.
	 */
	void Initialize();

	/**
	 * This is a function will be called by the public Load and it will provide it's own Record.
	 * The Load logic will be put in here. The existence of this function is purely for testing
	 * purposes (to Mock record).
	 */
	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

public:
	DBFile ();
    virtual ~DBFile();

    /**
	 * Creates a file at fpath of file_type heap, sorted, or tree. It will also create a file to
	 * store meta-data located at filepath.header.
	 * @param fpath		The path to the file that should be created.
	 * @param file_type	The type of the file as defined by ftype
	 * @param startup	Unknown
	 * @return			1 if successful and 0 if failure.
	 */
	virtual int Create (const char *fpath, fType file_type, void *startup);

    /**
	 * Opens the file and associated header located at fpath and fpath.header respectively. This
	 * function does not load any data from the file into memory.
	 * @param fpath	The path to the DBFile
	 * @return 		1 if successful and 0 if failure.
	 */
	virtual int Open (const char *fpath);

	/**
	 * Closes the file held by this object.
	 * @return 1 if successful and 0 if failure.
	 */
	virtual int Close ();

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
};

#endif
