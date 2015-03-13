/*
 * SelectPipe.h
 * The Select Operator
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SELECTPIPE_H_
#define INCLUDE_SELECTPIPE_H_

#include "RelationalOp.h"

class SelectPipe : public RelationalOp {
private:

public:
	SelectPipe();
	virtual ~SelectPipe();

	/**
	 * Compares each Record from inPipe with selOp and literal and puts any passing Records into
	 * outPipe. The work is done in a separate Thread
	 * @param inPipe	The source of Records being filtered
	 * @param outPipe	The destination for the Selected Records
	 * @param selOp		The CNF used to compare the Records from inPipe
	 * @param literal	The literal to be compared against the Records from inPipe
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);

	/**
	 * blocks the caller until the particular relational operator
	 * has run to completion
	 */
	virtual void WaitUntilDone ();

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);
};

#endif /* INCLUDE_SELECTPIPE_H_ */
