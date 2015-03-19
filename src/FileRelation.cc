/*
 * File.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "../include/FileRelation.h"

FileRelation::FileRelation(): Relation(), rfile(_Rfile), file(_File), config(_Config),
	comp(_Comp), relation(_Relation) {

}

FileRelation::FileRelation(RawFile &_rfile, File &_file, DBConfig &_config, ComparisonEngine &_comp,
		HeapDBFile &_relation): Relation(), rfile(_rfile), file(_file), config(_config),
				comp(_comp), relation(_relation) {

}

FileRelation::~FileRelation() {

}

bool FileRelation::Add(Record *rec) {
	relation.Add(*rec);
	return true;
}

bool FileRelation::Add(Pipe &in) {
	Record rec;
	return Add(in, rec);
}

bool FileRelation::Add(Pipe &in, Record &temp) {
	while(in.Remove(&temp)) {
		relation.Add(temp);
	}
	return true;
}

bool FileRelation::Add(Relation &rel) {
	Record *rec = NULL;
	rel.Reset(); // Make sure to get all the Records
	while(rel.GetNext(rec)) {
		relation.Add(*rec);
	}
	return true;
}

bool FileRelation::GetNext(Record *&rec) {
	return false;
}

void FileRelation::Reset() {
}

void FileRelation::Clear() {

}
