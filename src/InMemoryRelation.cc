/*
 * InMemory.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "../include/InMemoryRelation.h"

InMemoryRelation::InMemoryRelation(int memLimit): Relation(memLimit) {
	count = 0;
	memUsed = 0;
	index = 0;
}

InMemoryRelation::~InMemoryRelation() {
	// TODO Auto-generated destructor stub
}

bool InMemoryRelation::Add(Record *rec) {
	if(rec->Size() + memUsed <= memLimit) {
		memUsed += rec->Size();
		++count;

		// Check if vector needs to be resized
		if(relation.size() < count) relation.push_back(NULL);

		Record *temp = new Record;
		temp->Consume(rec);
		relation[count-1] = temp;
		return true;
	}
	return false;
}

bool InMemoryRelation::GetNext(Record *rec) {
	return true;
}

void InMemoryRelation::Reset() {

}

void InMemoryRelation::Clear() {

}
