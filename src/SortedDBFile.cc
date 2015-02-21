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
#include <iostream>
#include <string.h>
#include <stdio.h>

SortedDBFile::SortedDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *_f_path, SortInfo *_sortInfo):
GenericDBFile(file, rfile, config, comp), f_path(_f_path), sortInfo(_sortInfo) {

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

}

void SortedDBFile::Reset() {

}

void SortedDBFile::Initialize() {

}
