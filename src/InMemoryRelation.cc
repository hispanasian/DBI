/*
 * InMemory.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "../include/InMemoryRelation.h"

InMemoryRelation::InMemoryRelation(int memLimit): Relation(memLimit) {
	size = 0;
	memUsed = 0;
	index = 0;
}

InMemoryRelation::~InMemoryRelation() {
	Clear();
	relation.clear();
}

bool InMemoryRelation::Add(Record *rec) {
	if(rec->Size() + memUsed <= memLimit) {
		memUsed += rec->Size();
		++size;

		// Check if vector needs to be resized
		if(relation.size() < size) relation.push_back(NULL);

		Record *temp = new Record;
		temp->Consume(rec);
		relation[size-1] = temp;
		return true;
	}
	return false;
}

bool InMemoryRelation::GetNext(Record *&rec) {
	if(index < size) {
		rec = relation[index];
		++index;
		return true;
	}
	return false;
}

void InMemoryRelation::Reset() {
	index = 0;
}

void InMemoryRelation::Clear() {
	for(int i = 0; i < relation.size(); i++) {
		delete relation[i];
		relation[i] = NULL;
	}
	index = 0;
	size = 0;
	memUsed = 0;
}

int InMemoryRelation::MemUsed() {
	return memUsed;
}

void InMemoryRelation::SetMemLimit(int limit) {
	memLimit = limit;
}
