/*
 * PipedPage.h
 *
 * A PipedPage will emulate Page::GetFirst, Append, and EmptyItOut(). Calls to ToBinary or
 * FromBinary will do nothing.
 *  Created on: Feb 20, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_PIPEDPAGE_H_
#define INCLUDE_PIPEDPAGE_H_

#include "File.h"
#include "Pipe.h"

struct PipedPageData {
	Pipe &recs;
	File *file;
};

class PipedPage: public Page {
private:
	Pipe &recs;
public:
	// constructor
	PipedPage ();
	PipedPage (Pipe &recs);

	/**
	 * This is a tricky Constructor. It will take file and create a pthread that will open it as
	 * a heap (even if it is a sorted file, as long as things are added in order or read in order,
	 * it is effectively sorted), read from it, and put it into the _recs until there are no
	 * remaining files.
	 */
	PipedPage (Pipe &_recs, File *&file);
	virtual ~PipedPage ();

	/**
	 * Does nothing
	 */
	virtual void ToBinary (char *bits);

	/**
	 * Does nothing
	 */
	virtual void FromBinary (char *bits);

	/**
	 * Puts the next record in the Pipe in firstOne.
	 * @param firstOne	The Record that will hold the next Record in the Pipe.
	 * @return			A 1 if there was a Record in the pipe or a 0 if there are no remaining
	 * 					Records in the Pipe.
	 */
	virtual int GetFirst (Record *firstOne);

	/**
	 * Adds addMe to the pipe. addMe is consumed.
	 * @param addMe	The Record that will be added to the Pipe.
	 * @return		A 1 if a success, a 0 if failure.
	 */
	virtual int Append (Record *addMe);

	/**
	 * Shuts down the Pipe. Note that this could be dangerous if the Pipe was provided. Ensure that
	 * any Pipes feeding the provided Pipe are shut down as well.
	 */
	virtual void EmptyItOut ();
};




#endif /* INCLUDE_PIPEDPAGE_H_ */
