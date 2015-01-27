#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <sys/stat.h>
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
	int success = 1;
	// Only create a file if one does not currently exists (nor its header)
	if(FileExists(f_path) ||
			FileExists(strcat(f_path, ".header")) ||
			f_path == NULL) success = 0;
	else {
		file.Open(0, strcat(f_path, ".header"));
		config.AddKey("fType", "heap");
		config.Write(file);

		// Remove the file and header if there were any problems
		if(success == 0) {
			remove(f_path);
			remove(strcat(f_path, ".header"));
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

/**
 * Checks if a file located at path exists. This code was stolen from:
 * http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 * @param name	The name of the file
 * @return True if the file exists
 */
bool DBFile::FileExists(const std::string& file) {
  struct stat buffer;
  return (stat (file.c_str(), &buffer) == 0);
}
