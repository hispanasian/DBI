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

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile (): file(myFile), rfile(myRFile), config(myConfig) {
	cursorIndex = 0;
	lastIndex = 0;
	cursor = new Page();
	last = new Page();
}

DBFile::DBFile (File &otherFile, RawFile &otherRFile, DBConfig &otherConfig):
		file(otherFile), rfile(otherRFile), config(otherConfig) {
	cursorIndex = 0;
	lastIndex = 0;
	cursor = new Page();
	last = new Page();
}

DBFile::~DBFile () {
	delete cursor;
	delete last;
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
				FileExists(f_path) ||
				FileExists(header) ) {
			success = 0;
		}
		else {
			switch (f_type) {
			case heap:
				config.AddKey("fType", "heap");
				break;
			case sorted:
				config.AddKey("fType", "sorted");
				break;
			case tree:
				config.AddKey("fType", "tree");
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
				config.Clear(); // Clear any changes made if there was a failure
			}
		}
	}
	return success;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {

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

		if(!FileExists(f_path) || !FileExists(header)) success = false;
		else {
			// Begin
			file.Open(1, f_path);
			rawOpen = rfile.Open(header);
			success = success && rawOpen;
			success = success && config.Read(rfile);

			if(success) {
				const char * key = config.GetKey("fType").c_str();
				lastIndex = file.GetLength() - 1;
				file.GetPage(last, lastIndex);

				if(strcmp("heap", key) == 0) {
					// TODO: Implement
				}
				else if(strcmp("sorted", key) == 0) {
					// TODO: Implement
				}
				else if(strcmp("tree", key) == 0) {
					// TODO: Implement
				}
				else success = false;
			}
			if(!success) {
				if(rawOpen) rfile.Close(); // Closing an unopened RawFile segfaults
				file.Close();
				config.Clear(); // Clear any changes made if there was a failure
			}
		}
	}
	return success;
}

void DBFile::MoveFirst () {
	file.AddPage(last, lastIndex); // Write out last page
	file.GetPage(cursor, 0);
	cursorIndex = 0;
}

int DBFile::Close () {
	bool success = true;
	file.AddPage(last, lastIndex); // Write out last page
	file.Close();

	success &= config.Write(rfile);
	success &= rfile.Close();

	Reset();

	return success;
}

void DBFile::Add (Record &rec) {
	if(!last->Append(&rec)) {
		file.AddPage(last, lastIndex);
		last->EmptyItOut();
		if(!last->Append(&rec)) throw std::runtime_error("rec exceeds the Page size");
		lastIndex++;
	}
}

int DBFile::GetNext (Record &fetchme) {
	return 0;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	return 0;
}

void DBFile::Reset() {
	cursorIndex = 0;
	lastIndex = 0;
	cursor->EmptyItOut();
	last->EmptyItOut();
	config.Clear();
}
