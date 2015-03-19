/*
 * Sum.h
 *
 *  Created on: Mar 18, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SUM_H_
#define INCLUDE_SUM_H_

#include "Record.h"
#include "Pipe.h"
#include "RelationalOp.h"
#include "Function.h"

class Sum: public RelationalOp {
public:
	Sum();
	virtual ~Sum();

	/**
	 * Computes the sum of the Records from inPipe absed on computeMe and puts a Record of the
	 * appropriate type in outPipe.
	 * @param inPipe	The input pipe
	 * @param outPipe	The output pipe
	 * @param computeMe	The Function that will perform the actual computation
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe);

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * This method will do the single threaded work for Sum
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe);

	/**
	 * This method will do the single threaded work for Sum
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec);
};

struct SumData {
	Pipe &in;
	Pipe &out;
	Function &computeMe;
	Sum &op;
};

#endif /* INCLUDE_SUM_H_ */
