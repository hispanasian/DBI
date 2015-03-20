/*
 * JoinRelation.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_JOINRELATION_H_
#define INCLUDE_JOINRELATION_H_

#include "Relation.h"

class JoinRelation: public Relation {
friend class JoinRelationTest;
protected:
	Relation *delegate;
	bool memory;
public:
	JoinRelation(int memLimit);
	virtual ~JoinRelation();

	/**
	 * Adds rec to the relation
	 * @param rec		The Record that will be added to the Relation (this is consumed)
	 * @param return	true if the record was added with a in memory relation and false if it was
	 * 					added to a file relation.
	 */
	virtual bool Add(Record *rec);

	/**
	 * Returns the next Record in the Relation and puts it in rec. Returns false once we have run
	 * out of Records. rec should not be NULL and should be instantiated.
	 * @param rec	The Record that will be returned (this is consumed).
	 * @return		False if no Records remain
	 */
	virtual bool GetNext(Record *&rec);

	/**
	 * Returns the Relation to the beginning.
	 */
	virtual void Reset();

	/**
	 * Deletes Records from the Relation.
	 */
	virtual void Clear();

	/**
	 * Returns true if the Relation used is an in-memory relation and false if it is a file
	 * relation.
	 */
	virtual bool IsMemoryMode();
};

#endif /* INCLUDE_JOINRELATION_H_ */
