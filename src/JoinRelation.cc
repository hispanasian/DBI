/*
 * JoinRelation.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "JoinRelation.h"
#include "InMemoryRelation.h"
#include "FileRelation.h"

JoinRelation::JoinRelation(int memLimit): Relation(memLimit) {
	memory = true;

}

JoinRelation::~JoinRelation() {
	delete delegate;
}

bool JoinRelation::Add(Record *rec) {
	// This should only return false if it is a InMemoryRelation, in which case we now need to
	// switch it to be a FileRelation and change memory
	if(!delegate->Add(rec)) {
		memory = false;
		FileRelation *temp = new FileRelation();
		temp->Add(*delegate); // Populate the new relation
		delete delegate;
		delegate = temp;
		delegate->Add(rec); // Add the Record that failed to get added
	}
	return memory;
}

bool JoinRelation::GetNext(Record *&rec) {
	return true;
}

void JoinRelation::Reset() {

}

void JoinRelation::Clear() {

}

bool JoinRelation::IsMemoryMode() {
	return memory;
}
