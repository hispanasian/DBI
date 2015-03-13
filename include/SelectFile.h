/*
 * SelectFile.h
 *	The Select Operator that takes input from a DBFile
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SELECTFILE_H_
#define INCLUDE_SELECTFILE_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

class SelectFile: public RelationalOp {
public:
	SelectFile();
	virtual ~SelectFile();

	/**
	 * Compares each Record from inFile with selOp and literal and puts any passing Records into
	 * outPipe. The work is done in a separate Thread
	 * @param inFile	The source of Records being filtered
	 * @param outPipe	The destination for the Selected Records
	 * @param selOp		The CNF used to compare the Records from inPipe
	 * @param literal	The literal to be compared against the Records from inPipe
	 */
	virtual void Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * Select performs the work done by Run (effectively, this is a thread-less version of Run).
	 * This is where the Select logic is put.
	 */
	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);

	/**
	 * A Select that accepts a ComparisonEngine and Record (for testing purposes).
	 */
	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
};

struct SelectFileData {
	Pipe &in;
	Pipe &out;
	CNF &selOp;
	Record &literal;
	SelectFile &op;
};

#endif /* INCLUDE_SELECTFILE_H_ */
