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
	return false;
}

bool FileRelation::Add(Pipe &in) {
	return true;
}

bool FileRelation::Add(Relation &rel) {
	return true;
}

bool FileRelation::GetNext(Record *&rec) {
	return false;
}

void FileRelation::Reset() {
}

void FileRelation::Clear() {

}
