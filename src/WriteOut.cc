/*
 * WriteOut.cc
 *
 *  Created on: Mar 12, 2015
 *      Author: cvasquez
 */

#include "WriteOut.h"

WriteOut::WriteOut() {/* Do nothing */}

WriteOut::~WriteOut() {/* Do nothing */}

void WriteOut::Use_n_Pages(int n) {/* Do nothing */}

void WriteOut::Run(Pipe &inPipe, FILE *outFile, Schema &mySchema) {
	WriteOutData *data = new WriteOutData { inPipe, outFile, mySchema, *this };

	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		WriteOutData *data = (WriteOutData*)args;
		data->op.Write(data->in, data->out, data->schema);
		delete data;
	}, (void*) data);
	if(thread_id) {
    	cout << "Unable to create thread in WriteOut: " << thread_id << endl;
    }
}

void WriteOut::Write(Pipe &inPipe, FILE *outFile, Schema &mySchema) {
	RawFile file;
	Record rec;
	Write(inPipe, outFile, mySchema, file, rec);
}

void WriteOut::Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, RawFile &file, Record &rec) {
	file.Open(outFile);
	while(inPipe.Remove(&rec)) {
		file.Append(rec.ToString(&mySchema));
	}
}
