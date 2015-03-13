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

class DuplicateRemoval:RelationalOp {
private:
	int pageLimit;
public:
	DuplicateRemoval();
	virtual ~DuplicateRemoval();

	/**
	 * Removes the duplicate Records from inPipe and puts the unique Records in outPipe
	 * @param inPipe	The source of Records being filtered
	 * @param outPipe	The destination for the Selected Records
	 * @param mySchema
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema);

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * This is a single-threaded implementation of the Removal. Note that this method expects that
	 * inPipe is Sorted (ie, use BigQ)
	 */
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);

	/**
	 * This is where the logic of the Removal goes. This is meant for testing and the other Remove
	 * should be used
	 */
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, ComparisonEngine &comp, OrderMaker &order);

	/*
	 * Returns the pageLimit
	 */
	virtual int GetPageLimit();
};

#endif /* INCLUDE_DUPLICATEREMOVAL_H_ */
