/*
 * Relation.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_RELATION_H_
#define INCLUDE_RELATION_H_

#include "Defs.h"
#include "Record.h"

class Relation {
protected:
	int memLimit;

public:
	/**
	 * Creates a Relation with the provided memory limit
	 * @param memLimit	The maximum amount of memory that will be given to the Records (in bytes)
	 */
	Relation(int _memLimit = PAGE_SIZE) { memLimit = _memLimit; }
	virtual ~Relation() {};

	/**
	 * Adds rec to the relation and returns true if successful (typically if memLimit is exceeded)
	 * @param rec		The Record that will be added to the Relation (this is consumed)
	 * @param return	True if rec was successfully added
	 */
	virtual bool Add(Record *rec) { return true; }

	/**
	 * Returns the next Record in the Relation and puts it in rec. Returns false once we have run
	 * out of Records.
	 * @param rec	The Record that will be returned (this is consumed).
	 * @return		False if no Records remain
	 */
	virtual bool GetNext(Record *&rec) { return true; }

	/**
	 * Returns the Relation to the beginning.
	 */
	virtual void Reset() {}

	/**
	 * Deletes Records from the Relation.
	 */
	virtual void Clear() {}

	/**
	 * Returns the amount of memory used by this Relation (in bytes).
	 * @return	the amount of memory used by this Relation (in bytes).
	 */
	virtual int MemUsed();
};


#endif /* INCLUDE_RELATION_H_ */
