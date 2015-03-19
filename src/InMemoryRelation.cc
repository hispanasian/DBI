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
}

InMemoryRelation::~InMemoryRelation() {
	// TODO Auto-generated destructor stub
}

bool InMemoryRelation::Add(Record *rec) {
	if(rec->Size() + memUsed <= memLimit) {
		memUsed += rec->Size();
		Record *temp = new Record;
		temp->Consume(rec);
		relation.push_back(temp);
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
