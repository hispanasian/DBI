/*
 * WriteItOut.h
 * WritItOut will write Records to a FILE
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_WRITEITOUT_H_
#define INCLUDE_WRITEITOUT_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Schema.h"
#include "Record.h"
#include "RawFile.h"

class WriteItOut: public RelationalOp {
public:
	WriteItOut();
	virtual ~WriteItOut();

	/**
	 * Writes the Records from inPipe to outFile with the given mySchema
	 * @param inPipe	The input Pipe
	 * @param outFile	The output file
	 * @param mySchema	The Schema of the Records
	 */
	void Run (Pipe &inPipe, FILE *outFile, Schema &mySchema);

	/**
	 * This method does nothing
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * Write performs the work done by Run (effectively, this is a thread-less version of Run).
	 * This is where the Write logic is put.
	 */
	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema);

	/**
	 * A Write that accepts a RawFile and Record (for testing purposes).
	 */
	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, RawFile &file, Record &rec);
};

struct WriteItOutData {
	Pipe &in;
	FILE *out;
	Schema &schema;
	WriteItOut &op;
};

#endif /* INCLUDE_WRITEITOUT_H_ */
