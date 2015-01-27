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

DBFile::DBFile () {
	curPage = 0;
}

DBFile::~DBFile () {

}

int DBFile::Create (char *f_path, fType f_type, void *startup) {return Create(f_path, f_type, startup, file); }

int DBFile::Create (char *f_path, fType f_type, void *startup, File file) {
	return 0;
}

void DBFile::Load (Schema &f_schema, char *loadpath) { Load(f_schema, loadpath, file); }

void DBFile::Load (Schema &f_schema, char *loadpath, File file) {

}

int DBFile::Open (char *f_path) { return Open(f_path, file); }

int DBFile::Open (char *f_path, File file) {
	return 0;
}

void DBFile::MoveFirst () { MoveFirst(file); }

void DBFile::MoveFirst (File file) {
}

int DBFile::Close () { return Close(file); }

int DBFile::Close (File file) {
  return 0;
}

void DBFile::Add (Record &rec) { Add(rec, file); }

void DBFile::Add (Record &rec, File file) {
}

int DBFile::GetNext (Record &fetchme) { return GetNext(fetchme, file); }

int DBFile::GetNext (Record &fetchme, File file) {
	return 0;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) { return GetNext(fetchme, cnf, literal, file); }

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal, File file) {
	return 0;
}

/**
 * Checks if a file located at path exists. This code was stolen from:
 * http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 * @param name	The name of the file
 * @return True if the file exists
 */
inline bool FileExists(const std::string& file) {
  struct stat buffer;
  return (stat (file.c_str(), &buffer) == 0);
}
