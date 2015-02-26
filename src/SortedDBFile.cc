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
	cursor = new Page();
	cursorIndex = 0;
	rwState = Reading;
	getNextState = NoCNF;
}

SortedDBFile::SortedDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *_f_path, SortInfo *_sortInfo):
GenericDBFile(file, rfile, config, comp), f_path(_f_path), sortInfo(_sortInfo), cursor(new Page()) {
	in = NULL;
	out = NULL;
	rwState = Reading;
	getNextState = NoCNF;	
	cursorIndex = 0;
	cursor = new Page();
}

SortedDBFile::~SortedDBFile () {
	Reset();
	delete cursor;
}

void SortedDBFile::Load (Schema &f_schema, char *loadpath) {

}

void SortedDBFile::Load (Schema &f_schema, char *loadpath, Record &record) {

}

void SortedDBFile::MoveFirst () {
	// if state is writing, flush
	Flush(); // flush checks for writing state
	
	// read the 0th page into mem if it exists
	if(GetLength() > 0) file.GetPage(cursor, 0); // Get Page if it exists
	// otherwise empty the pointer
	else cursor -> EmptyItOut(); // Empty out current Page if no Page exists
	
	// Invariants
	// set the cursor index to 0
	cursorIndex = 0;
	// set the GetNexCNF state to NoCNF
	getNextState = NoCNF;
	// set the RW state to Reading
	rwState = Reading;
}

void SortedDBFile::Add (Record &rec) {

}

int SortedDBFile::GetNext (Record &fetchme) {
	return 0;
}

int SortedDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal, ComparisonEngine &comp){
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
	if(rwState == Writing) {
		File file;
		Flush(file);
		rwState = Reading;
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

	in->ShutDown(); // Stop the Pipe so we don't block
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

	// Cleanup/Reset state
	Reset();
	Initialize();
}

void SortedDBFile::Reset() {
	cursor -> EmptyItOut();
	cursorIndex = 0;
	if(in != NULL) in->ShutDown();

	// Make sure that BigQ finishes up with in
	Record *rec = new Record();
	if(out != NULL) {
		while(out->Remove(rec) != 0) {/* empty that thing out and wait for BigQ to shut it down */}
	}
	delete in;
	delete out;
	delete rec;
	in = NULL;
	out = NULL;
}

void SortedDBFile::Initialize() {
	bool create = (in == NULL || out == NULL);
	if(in == NULL) in = new Pipe(PIPE_SIZE);
	if(out == NULL) out = new Pipe(PIPE_SIZE);
	if(create) BigQ(*in, *out, *(sortInfo->myOrder), sortInfo->runLength);
}

bool SortedDBFile::BinarySearch(Record &literal, OrderMaker &query) {
	Record rec;
	Page page;
	ComparisonEngine comp;
	return BinarySearch(literal, query, comp, rec, page);
}

bool SortedDBFile::BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page) {
	off_t start = cursorIndex;
	off_t end = GetLength() - 1;
	off_t mid;

	while(start < end) {
		mid = (start + end + 1)/2; // super important to take 'ceiling'
		GetBSTPage(page, mid);
		if(page.GetFirst(&rec) == 0) ++start; // This should only happen when page is cursor and cursor is empty
		else if(comp.Compare(&rec, &literal, &query) <= 0) start = mid;
		else /* comp.Compare(&rec, &literal, &query) > 0 */ end = mid - 1;
	}

	// Now it must be the case that mid == start == end
	// mid is an index to a page that:
	// a. contains a Record that conforms to the query/literal pair or
	// b. precedes a Page with a Record that conforms to the query/literal pair
	// Find the Page/Record and set it as the cursor if it exists.
	return FindValidRecord(literal, query, mid);
}

void SortedDBFile::GetBSTPage(Page &page, off_t index) {
	if(index == cursorIndex) {
		char *bits = new (std::nothrow) char[PAGE_SIZE];
		cursor->ToBinary(bits);
		page.FromBinary(bits);
	}
	else file.GetPage(&page, index);
}

bool SortedDBFile::FindValidRecord(Record &literal, OrderMaker &query, off_t index) {
	Record rec;
	Page page;
	Page buff;
	ComparisonEngine comp;
	return FindValidRecord(literal, query, index, rec, page, buff, comp);
}

bool SortedDBFile::FindValidRecord(Record &literal, OrderMaker &query, off_t index, Record &rec,
		Page &page, Page &buff, ComparisonEngine &comp) {

	int c;

	// Look for a valid record in the first page
	GetBSTPage(page, index);
	while(page.GetFirst(&rec) != 0 && (c = comp.Compare(&rec, &literal, &query)) <= 0) {
		if(c == 0) break;
	}

	// Look for a valid Record in the following page if none exists in the current one. If
	// c != 0, then the previous loop failed because it ran out of Records
	if(c < 0 && index < GetLength() - 1) {
		++index;
		GetBSTPage(page, index);
		while(page.GetFirst(&rec) != 0 && (c = comp.Compare(&rec, &literal, &query)) <= 0) {
			if(c == 0) break;
		}
	}

	// Again, if c != 0, then the previous loop ran out of Records without finding a matching Record
	if(c != 0) return false;
	// Copy remaining Records into buff and update cursor and cursorIndex
	buff.Append(&rec); // Add the first correct Record
	while(page.GetFirst(&rec) != 0) { buff.Append(&rec); }

	// Update cursor
	char *bits = new (std::nothrow) char[PAGE_SIZE];
	buff.ToBinary(bits);
	cursor->FromBinary(bits);
	cursorIndex = index;

	return true;
}
