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

	config.Clear(); // Obligatory clear

	if(f_path == NULL) success = false;
	else {
		// Create header path
		std::string buf(f_path);
		buf.append(".header");
		char *header = (char *)buf.c_str(); // watch out... make sure casting off the const-ness does not cause problems

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
	return 0;
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
