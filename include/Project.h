/*
 * Project.h
 *
 *  Created on: Mar 13, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_PROJECT_H_
#define INCLUDE_PROJECT_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Record.h"

class Project {
public:
	Project();
	virtual ~Project();

	/**
	 * Projects the Rcords from inPipe to outPipe according to keepMe, numAttsInput, and
	 * numAttsOutput.
	 * @param inPipe			The Records that will be Projected
	 * @param outPipe			The Projected Records
	 * @param keepMe			The attributes that should be kept from the Records in inPipe
	 * @parma numAttsInput		The number of attributes in the Records from inPipe
	 * @param numAttsOutput		The number of attributes in keepMe
	 */
	virtual void Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * Work performs the work that will be threaded in Run.
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);

	/**
	 * This is where the logic for the Project method will be. This exists mainly for testing.
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec);
};

struct ProjectData {
	Pipe &in;
	Pipe &out;
	int *keepMe;
	int numAttsInput;
	int numAttsOutput;
};

#endif /* INCLUDE_PROJECT_H_ */
