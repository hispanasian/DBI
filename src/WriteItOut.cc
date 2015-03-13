/*
 * WriteItOut.cc
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#include "WriteItOut.h"

WriteItOut::WriteItOut() {/* Do nothing */}

WriteItOut::~WriteItOut() {/* Do nothing */}

void WriteItOut::Use_n_Pages(int n) {/* Do nothing */}

void WriteItOut::Run(Pipe &inPipe, FILE *outFile, Schema &mySchema) {
	WriteItOutData *data = new WriteItOutData { inPipe, outFile, mySchema, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		WriteItOutData *data = (WriteItOutData*)args;
		data->op.Write(data->in, data->out, data->schema);
		delete data;
	}, (void*) data);
}

void WriteItOut::Write(Pipe &inPipe, FILE *outFile, Schema &mySchema) {
	RawFile file;
	Record rec;
	Write(inPipe, outFile, mySchema, file, rec);
}

void WriteItOut::Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, RawFile &file, Record &rec) {
	file.Open(outFile);
	while(inPipe.Remove(&rec)) {
		file.Append(rec.ToString(&mySchema));
	}
}
