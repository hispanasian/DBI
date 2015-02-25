#ifndef GENERICDBFILE_H
#define GENERICDBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "RawFile.h"
#include "DBConfig.h"
#include <sys/stat.h>

/**
 *	GenericDBFile is an interface that will be used by DBFile to utilize various DBFile types (heap,
 *	sorted, and tree)
 */
class GenericDBFile {
friend class DBFile;
friend class MockGenericDBFile;
protected:
	File &file;
	RawFile &rfile;
	DBConfig &config;
	ComparisonEngine &comp;

	/**
	 * Returns the length of file.
	 */
	int GetLength();

	/**
	 * This is a function will be called by the public Load and it will provide it's own Record.
	 * The Load logic will be put in here. The existence of this function is purely for testing
	 * purposes (to Mock record).
	 */
	virtual void Load (Schema &myschema, char *loadpath, Record &record) = 0;
	GenericDBFile();

public:
	GenericDBFile(File &_file, RawFile &_rfile, DBConfig &_config, ComparisonEngine &_comp);
    virtual ~GenericDBFile();

	/**
	 * Moves the pointer from the current record to the first record of the first page.
	 */
	virtual void MoveFirst () = 0;

	/**
	 * Adds addMe to the last page in memory. Writes are not immediately readable. In order to get
	 * the most immediate written records, you must call MoveFirst.
	 * @param addme	The Record being added to DBFile
	 */
	virtual void Add (Record &addme) = 0;

	/**
	 * Puts the current record in fetchme and moves the pointer to the next record. Upon use, the
	 * record referenced by fetchme is consumed and the new record in fetchme is removed from DB
	 * File.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme) = 0;

	/**
	 * Looks for the next record that passes applyMe when applied between a Record in DBFile and
	 * literal. This function increments the pointer accordingly.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @param applyMe	The CNF that will be applied between the Record(s) in DBFile and literal
	 * @param literal	The Record that will be compared with the Records in DBFile.
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal) = 0;

	/**
	 * Writes out any changes to disk
	 */
	virtual void Flush()= 0;

	/**
	 * Initializes this DBFile
	 */
	virtual void Initialize() = 0;

	/**
	 * Puts DBFile into an initial state.
	 */
	virtual void Reset() = 0;
};

#endif
