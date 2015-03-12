/*
 * RelationalOp.h
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_RELATIONALOP_H_
#define INCLUDE_RELATIONALOP_H_

class RelationalOp {
public:
	// blocks the caller until the particular relational operator
	// has run to completion
	virtual void WaitUntilDone () = 0;
	// tells how much internal memory the operation can use
	virtual void Use_n_Pages (int n) = 0;

	virtual ~RelationalOp();
};

#endif /* INCLUDE_RELATIONALOP_H_ */
