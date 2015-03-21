/*
 * FileRelation.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_FILERELATION_H_
#define INCLUDE_FILERELATION_H_

#include "Relation.h"
#include "Record.h"
#include "ComparisonEngine.h"
#include "DBConfig.h"
#include "RawFile.h"
#include "File.h"
#include "HeapDBFile.h"
#include "Pipe.h"

class FileRelation: public Relation {
friend class FileRelationTest;

protected:
	RawFile &rfile;
	RawFile _Rfile;
	File &file;
	File _File;
	DBConfig &config;
	DBConfig _Config;
	ComparisonEngine &comp;
	ComparisonEngine _Comp;
	HeapDBFile &relation;
	HeapDBFile _Relation = HeapDBFile(file, rfile, config, comp);
	char name[9] = { 'F', 'R', 'X', 'X', 'X', 'X', 'X', 'X', '\0' }; //= "FileRelationXXXXXX";

	FileRelation(RawFile &_rfile, File &_file, DBConfig &_config, ComparisonEngine &_comp, HeapDBFile &_relation);
	/**
	 * Adds all the Records from in to the Relation
	 * @param in		The input Pipe
	 * @param temp		The Record that will be used to remove Records from Pipe
	 * @param return	Always returns true
	 */
	virtual bool Add(Pipe &in, Record &temp);

public:
	/**
	 * Creates a Relation that will be written to disk (temporarily). Anything written on disk will
	 * be deleted when this object is deleted. This will hold one Page worth of Records.
	 */
	FileRelation();
	virtual ~FileRelation();

	/**
	 * Adds rec to the relatio
	 * @param rec		The Record that will be added to the Relation (this is consumed)
	 * @param return	Always returns true
	 */
	virtual bool Add(Record *rec);

	/**
	 * Adds all the Records from in to the Relation
	 * @param in		The input Pipe
	 * @param return	Always returns true
	 */
	virtual bool Add(Pipe &in);

	/**
	 * Adds all the Records from rel to this Relation
	 * @param rel	The Relation with the Records that will be added to this Relation
	 * @return		Always returns true
	 */
	virtual bool Add(Relation &rel);

	/**
	 * Returns the next Record in the Relation and puts it in rec. Returns false once we have run
	 * out of Records. Note that rec should not be NULL and should have been allocated.
	 * @param rec	The Record that will be returned (this is consumed).
	 * @return		False if no Records remain
	 */
	virtual bool GetNext(Record *&rec);

	/**
	 * Returns the Relation to the first Record.
	 */
	virtual void Reset();

	/**
	 * Deletes the Records in memory and clears the file
	 */
	virtual void Clear();

	/**
	 * Returns the amount of memory used by this Relation (in bytes). This will simply be PAGE_SIZE
	 * @return	the amount of memory used by this Relation (in bytes).
	 */
	virtual int MemUsed();
};


#endif /* INCLUDE_FILERELATION_H_ */
