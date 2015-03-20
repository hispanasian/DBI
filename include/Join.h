/*
 * Join.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_JOIN_H_
#define INCLUDE_JOIN_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "Record.h"
#include "JoinRelation.h"
#include "InMemoryRelation.h"

class Join: public RelationalOp {
friend class JoinTest;
friend class PartialMockJoin;

protected:
	int pageLimit;
	int memLimit;

	/**
	 * This method holds the logic for Work
	 */
	void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR);

	/**
	 * This method performs a sort merge join and it expects the Records from pipeL and pipeR to be
	 * sorted. This will find groups from pipeL and pipeR and put them itno relR and relS
	 * (respectively) and then join those relations.
	 * @param pipeL		The left input pipe
	 * @param pipeR		The right input pipe
	 * @param outPipe	The output pipe
	 * @param orderL	The ordermaker associated with pipeL
	 * @param orderR	The ordermaker associated with pipeR
	 * @param relR		The relation that will hold the groups from pipeL
	 * @param relS		The relation that will hold the groups from pipeR
	 * @param tempL		A temp Record that will be used to deal with records from pipeL and relR
	 * @param tempR		A temp Record that will be used to deal with records from pipeR and relS
	 */
	void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR, JoinRelation &relR, JoinRelation &relS, Record &tempL, Record &tempR);

	/**
	 * This method performs a block nested loop join. This join is performed when the CNF can't be
	 * implemented to perform a sort merge join. The right pipe will be put into rel S. This is the
	 * relation we hope will fit in memory (we make a guess that this is the smallest relation). We
	 * then fit as many Records as we can into relR and try to merge the Records in both relations.
	 * @param pipeL		The left input pipe
	 * @param pipeR		The right input pipe
	 * @param outPipe	The output pipe
	 * @param selOp		The cnf
	 * @param literal	The literal used to compare Records to see if they should be joined
	 * @param relR		The R relation
	 * @param relS		The S relation
	 * @param rec		The record that will be used to merge Records and put them in the outPipe
	 */
	void BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal, JoinRelation &relS, InMemoryRelation &relR);

public:
	Join();
	virtual ~Join();

	/**
	 * Sets the page limit
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * This will join the Records from inPipeL and inPipeR and put the joined records into outPipe.
	 * This work will be done in a separate thread
	 * @param inPipeL	The Records in the left relation
	 * @param inPipeR	The Records in the right relation
	 * @param outPipe	The pipe that will hold the joine records
	 * @param selOp		The CNF that will define the attributes on which to join
	 * @param literal	The literal that will be used to see if a join should be made
	 */
	virtual void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);

	/**
	 * This method performs the Join in a single-threaded manner.
	 */
	virtual void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);

	/**
	 * This is the SortMergeJoin that should be called by any method that wants to perform a
	 * sort merge join. This method will sort pipeL and pipeR
	 */
	virtual void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR);

	/**
	 * This is the BlockNestedLoopJoin that should be called by any method that wants to perform a
	 * block nested loop join.
	 */
	virtual void BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal);
};

struct JoinData {
	Pipe &inL;
	Pipe &inR;
	Pipe &out;
	CNF &selOp;
	Record &literal;
	Join &op;
};

#endif /* INCLUDE_JOIN_H_ */
