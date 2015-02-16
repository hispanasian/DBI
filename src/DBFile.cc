#include "GenericDBFile.h"
#include "HeapDBFile.h"
#include "TreeDBFile.h"
#include "SortedDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile (): file(myFile), rfile(myRFile), config(myConfig), comp(myComp) {
	delegate = NULL;
}

DBFile::DBFile (File &otherFile, RawFile &otherRFile, DBConfig &otherConfig, ComparisonEngine &otherComp):
		file(otherFile), rfile(otherRFile), config(otherConfig), comp(otherComp) {
	delegate = NULL;
}

DBFile::~DBFile () {
	delete delegate;
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
	bool success = true;
	bool rawOpen = false;

	Reset();

	if(f_path == NULL) success = false;
	else {
		// Create header path
		std::string buf(f_path);
		buf.append(".header");
		const char *header = buf.c_str();

		if(f_path == NULL ||
				rfile.FileExists(f_path) ||
				rfile.FileExists(header) ) {
			success = 0;
		}
		else {
			switch (f_type) {
			case heap:
				config.AddKey("fType", "heap");
				delegate = new HeapDBFile(file, rfile, config, comp);
				break;
			case sorted:
				config.AddKey("fType", "sorted");
				delegate = new SortedDBFile(file, rfile, config, comp);
				break;
			case tree:
				config.AddKey("fType", "tree");
				delegate = new TreeDBFile(file, rfile, config, comp);
				break;
			}
			file.Open(0, f_path);

			// If the preceeding operation failed, do not perform the succeeding op... yes, this looks funky
			rawOpen = rfile.Open(header);
			success = success && rawOpen;
			success = success && config.Write(rfile);
			// Remove the file and header and undo any changes made to the config if there were any
			// problems
			if(!success) {
				file.Close();
				if(rawOpen) rfile.Close(); // Closing an unopened RawFile segfaults
				remove(f_path);
				remove(header);
				Reset();
			}
			else delegate->Initialize();
		}
	}
	return success;
}

int DBFile::Open (char *f_path) {
	bool success = true;
	bool rawOpen = false;

	Reset();

	if(f_path == NULL) success = false;
	else {
		// Create header path
		std::string buf(f_path);
		buf.append(".header");
		const char *header = buf.c_str();

		if(!rfile.FileExists(f_path) || !rfile.FileExists(header)) success = false;
		else {
			// Begin
			file.Open(1, f_path);
			rawOpen = rfile.Open(header);
			success = success && rawOpen;
			success = success && config.Read(rfile);

			if(success) {
				const char * key = config.GetKey("fType").c_str();

				if(strcmp("heap", key) == 0) {
					delegate = new HeapDBFile(file, rfile, config, comp);
				}
				else if(strcmp("sorted", key) == 0) {
					// TODO: Implement
				}
				else if(strcmp("tree", key) == 0) {
					// TODO: Implement
				}
				else success = false;
				if(success) Initialize();
			}
			if(!success) {
				if(rawOpen) rfile.Close(); // Closing an unopened RawFile segfaults
				file.Close();
				Reset();
			}
		}
	}
	return success;
}

int DBFile::Close () {
	bool success = true;
	delegate->Flush(); // Write out data
	file.Close();

	success &= config.Write(rfile);
	success &= rfile.Close();

	Reset();
	return success;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
	Record record;
	Load(f_schema, loadpath);
}

void DBFile::Load (Schema &f_schema, char *loadpath, Record &record) { delegate->Load(f_schema, loadpath, record); }


void DBFile::MoveFirst () { delegate->MoveFirst(); }

void DBFile::Add (Record &rec) { delegate->Add(rec); }

int DBFile::GetNext (Record &fetchme) { return delegate->GetNext(fetchme); }

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) { return delegate->GetNext(fetchme, cnf, literal); }

void DBFile::Reset() {
	if(delegate != NULL) delegate->Reset();
	config.Clear();
	delete delegate;
	delegate = NULL;
}

void DBFile::Initialize() { delegate->Initialize(); }
