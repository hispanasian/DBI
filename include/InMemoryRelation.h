/*
 * InMemory.h
 *	An in-memory representation of a Relation of Records. This uses a vector to keep the Records
 *	in-memory
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_INMEMORYRELATION_H_
#define INCLUDE_INMEMORYRELATION_H_

#include "Relation.h"
#include <vector>

class InMemoryRelation: public Relation {
friend class InMemoryRelationTest;

private:
	std::vector<Record*> relation;
	int count;
	int memUsed;

public:
	InMemoryRelation(int memLimit);
	virtual ~InMemoryRelation();

	/**
	 * Adds rec to the relation and returns true if memLimit is not exceeded
	 * @param rec		The Record that will be added to the Relation (this is consumed)
	 * @param return	True if rec was successfully added
	 */
	virtual bool Add(Record *rec);

	/**
	 * Returns the next Record in the Relation and puts it in rec. Returns false once we have run
	 * out of Records.
	 * @param rec	The Record that will be returned (this is consumed).
	 * @return		False if no Records remain
	 */
	virtual bool GetNext(Record *rec);

	/**
	 * Returns the Relation to the beginning.
	 */
	virtual void Reset();

	/**
	 * Deletes Records from the Relation.
	 */
	virtual void Clear();
};

#endif /* INCLUDE_INMEMORYRELATION_H_ */
