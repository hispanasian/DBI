/*
 * DuplicateRemoval.h
 *
 *  Created on: Mar 13, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_DUPLICATEREMOVAL_H_
#define INCLUDE_DUPLICATEREMOVAL_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

class DuplicateRemoval: public RelationalOp {
private:
	int pageLimit;
public:
	DuplicateRemoval();
	virtual ~DuplicateRemoval();

	/**
	 * Removes the duplicate Records from inPipe and puts the unique Records in outPipe
	 * @param inPipe	The source of Records being filtered
	 * @param outPipe	The destination for the Selected Records
	 * @param mySchema	The Schema of the Records in inPipe
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema);

	/**
	 * Removes the duplicate Records from inPipe and puts the unique Records in outPipe
	 * @param inPipe		The source of Records being filtered
	 * @param outPipe		The destination for the Selected Records
	 * @param mySchema		The Schema of the Records in inPipe
	 * @param duplicates	The Schema of the duplicates that we want to remove
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Schema &duplicates);

	/**
	 * Sets the number of pages to use in the BigQ for soring
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * This is a single-threaded implementation of the Removal. Note that this method expects that
	 * inPipe is Sorted (ie, use BigQ)
	 */
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);

	/**
	 * This is a single-threaded implementation of the Removal. Note that this method expects that
	 * inPipe is Sorted (ie, use BigQ)
	 */
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Schema &duplicates);

	/**
	 * This is where the logic of the Removal goes. This is meant for testing and the other Remove
	 * should be used
	 */
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order);

	/*
	 * Returns the pageLimit
	 */
	virtual int GetPageLimit();
};

struct DuplicateRemovalData {
	Pipe &in;
	Pipe &out;
	Schema &schema;
	Schema &duplicates;
	DuplicateRemoval &op;
};


#endif /* INCLUDE_DUPLICATEREMOVAL_H_ */
