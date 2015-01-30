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
	curPage = 0;
}

DBFile::DBFile (File &otherFile, RawFile &otherRFile, DBConfig &otherConfig):
		file(otherFile), rfile(otherRFile), config(otherConfig) {
	curPage = 0;
}

DBFile::~DBFile () {

}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
	bool success = true;
	bool rawOpen = false;

	curPage = 0;	// Reset Page offset
	config.Clear(); // Obligatory clear

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

	curPage = 0;	// Reset Page offset
	config.Clear(); // Obligatory clear

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
}

int DBFile::Close () {
	return 0;
}

void DBFile::Add (Record &rec) {
}

int DBFile::GetNext (Record &fetchme) {
	return 0;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	return 0;
}
