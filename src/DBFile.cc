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

}

DBFile::~DBFile () {

}

int DBFile::Create (char *f_path, fType f_type, void *startup) {

}

void DBFile::Load (Schema &f_schema, char *loadpath) {
}

int DBFile::Open (char *f_path) {
}

void DBFile::MoveFirst () {
}

int DBFile::Close () {
  return 0;
}

void DBFile::Add (Record &rec) {
  rec.Project(0,0,0);
}

int DBFile::GetNext (Record &fetchme) {
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
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
