/*
 * InMemory.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "../include/InMemoryRelation.h"

InMemoryRelation::InMemoryRelation(int memLimit): Relation(memLimit) {

	// TODO Auto-generated constructor stub

}

InMemoryRelation::~InMemoryRelation() {
	// TODO Auto-generated destructor stub
}

bool InMemoryRelation::Add(Record *rec) {
	return true;
}

bool InMemoryRelation::GetNext(Record *rec) {
	return true;
}

void InMemoryRelation::Reset() {

}

void InMemoryRelation::Clear() {

}
