#include "../include/GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

GenericDBFile::GenericDBFile(): file(myFile), rfile(myRFile), config(myConfig), comp(myComp){}

GenericDBFile::GenericDBFile(File &_file, RawFile &_rfile, DBConfig &_config,
		ComparisonEngine &_comp): file(_file), rfile(_rfile), config(_config), comp(_comp) {

}

GenericDBFile::~GenericDBFile() {

}

int GenericDBFile::GetLength() {
	off_t zero = 0;
	return max(zero, file.GetLength()-1);
}

void  GenericDBFile::Load (Schema &myschema, const char *loadpath, Record &record){}


/**
 * Moves the pointer from the current record to the first record of the first page.
 */
// virtual void MoveFirst () = 0;
void GenericDBFile::MoveFirst (){}

/**
 * Adds addMe to the last page in memory. Writes are not immediately readable. In order to get
 * the most immediate written records, you must call MoveFirst.
 * @param addme	The Record being added to DBFile
 */
// virtual void Add (Record &addme) = 0;
void GenericDBFile::Add (Record &addme){}

/**
 * Puts the current record in fetchme and moves the pointer to the next record. Upon use, the
 * record referenced by fetchme is consumed and the new record in fetchme is removed from DB
 * File.
 * @param fetchme	A reference that will be used to return the current Record
 * @return			0 if there is no valid record to be returned.
 */
// virtual int GetNext (Record &fetchme) = 0;
int GenericDBFile::GetNext (Record &fetchme){return 0;}

/**
 * Looks for the next record that passes applyMe when applied between a Record in DBFile and
 * literal. This function increments the pointer accordingly.
 * @param fetchme	A reference that will be used to return the current Record
 * @param applyMe	The CNF that will be applied between the Record(s) in DBFile and literal
 * @param literal	The Record that will be compared with the Records in DBFile.
 * @return			0 if there is no valid record to be returned.
 */
// virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal) = 0;
int GenericDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal){return 0;}

/**
 * Writes out any changes to disk
 */
// virtual void Flush()= 0;
void GenericDBFile::Flush(){}

/**
 * Initializes this DBFile
 */
// virtual void Initialize() = 0;
void GenericDBFile::Initialize(){}

/**
 * Puts DBFile into an initial state.
 */
// virtual void Reset() = 0;
void GenericDBFile::Reset(){}
