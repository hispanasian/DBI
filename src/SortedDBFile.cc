#include "../include/SortedDBFile.h"
#include "../include/GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include "../include/HeapDBFile.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

SortedDBFile::SortedDBFile(): GenericDBFile() {
	sortInfo = NULL;
	f_path = NULL;
	in = NULL;
	out = NULL;
	recordAdded = false;
}

SortedDBFile::SortedDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *_f_path, SortInfo *_sortInfo):
GenericDBFile(file, rfile, config, comp), f_path(_f_path), sortInfo(_sortInfo) {
	in = NULL;
	out = NULL;
	recordAdded = false;
	Initialize();
}

SortedDBFile::~SortedDBFile () {

}

void SortedDBFile::Load (Schema &f_schema, char *loadpath) {

}

void SortedDBFile::Load (Schema &f_schema, char *loadpath, Record &record) {

}

void SortedDBFile::MoveFirst () {

}

void SortedDBFile::Add (Record &rec) {

}

int SortedDBFile::GetNext (Record &fetchme) {
	return 0;
}

int SortedDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	while(this->GetNext(fetchme)) {
		if(comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
	}
	return 0;
}

void SortedDBFile::Flush() {
	if(recordAdded) {
		File file;
		Flush(file);
	}
}

void SortedDBFile::Flush(File &temp) {
	char *tempname = "tempXXXXXX";
	rfile.MakeTemp(tempname);
	string buf(f_path);
	buf.append(".bin");
	char *name = (char*)buf.c_str();
	RawFile temprfile;
	DBConfig tempconfig;
	ComparisonEngine tempcomp;

	// Here's where the real work gets done to merge the Records in bigq and the Records in file.
	temp.Open(0, tempname);
	HeapDBFile db = HeapDBFile(temp,temprfile, tempconfig, tempcomp);
	db.Initialize();
	Flush(db);

	// Cleanup and re-assign files
	temp.Close();
	file.Close();
	if(rfile.Remove(name) != 0) throw runtime_error("SortedDBFile::Flush failed to remove a file");;
	if(rfile.Rename(tempname, name) != 0) throw runtime_error("SortedDBFile::Flush failed to rename a file");
	file.Open(1, name);
}

void SortedDBFile::Flush(HeapDBFile &temp) {
	Pipe p = Pipe(PIPE_SIZE);
	PipedPage p1 = PipedPage(p, &file);
	PipedPage p2 = PipedPage(*in);
	pthread_t worker;
	Pipe sortedrecs;
	int runlen = 1;
	TPMMS tpmms = TPMMS(sortedrecs, sortedrecs, *(sortInfo->myOrder), runlen);
	MergeData *data = new MergeData { &p1, &p2, &tpmms };

	// Spin up a thread to do the sorting
	int t = pthread_create(&worker, NULL, [] (void* args) -> void* {
		MergeData *data = (MergeData*)args;
		data->tpmms->Merge(data->p1, data->p2);
	}, (void*)data);

	// Read in the sorted Records into temp
	Record rec;
	while(sortedrecs.Remove(&rec) != 0) {
		temp.Add(rec);
	}
	temp.Flush(); // Write out any remaining Records
}

void SortedDBFile::Reset() {
	if(in != NULL) in->ShutDown();

	// Make sure that BigQ finishes up with in
	Record *rec = new Record();
	if(out != NULL) {
		while(out->Remove(rec) != 0) {/* empty that thing out */}
		out->ShutDown();
	}
	delete in;
	delete out;
	delete rec;
	in = NULL;
	out = NULL;
}

void SortedDBFile::Initialize() {
	if(in == NULL) in = new Pipe(PIPE_SIZE);
	if(out == NULL) out = new Pipe(PIPE_SIZE);
}
