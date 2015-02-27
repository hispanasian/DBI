#include "../include/TreeDBFile.h"
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

TreeDBFile::TreeDBFile(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp):
GenericDBFile(file, rfile, config, comp) {

}

TreeDBFile::~TreeDBFile () {

}

void TreeDBFile::Load (Schema &f_schema, char *loadpath) {

}

void TreeDBFile::Load (Schema &f_schema, char *loadpath, Record &record) {

}

void TreeDBFile::MoveFirst () {

}

void TreeDBFile::Add (Record &rec) {

}

int TreeDBFile::GetNext (Record &fetchme) {
	return 0;
}

int TreeDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	while(this->GetNext(fetchme)) {
		if(comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
	}
	return 0;
}

void TreeDBFile::Flush() {

}

void TreeDBFile::Reset() {

}

void TreeDBFile::Initialize() {

}
