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
	delegate = new InMemoryRelation(memLimit);
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

bool JoinRelation::Populate(Pipe &in) {
	Record rec;
	while(in.Remove(&rec)) {
		Add(&rec);
	}
	return memory;
}

bool JoinRelation::GetNext(Record *&rec) {
	return delegate->GetNext(rec);
}

void JoinRelation::Reset() {
	return delegate->Reset();
}

void JoinRelation::Clear() {
	return delegate->Clear();
}

bool JoinRelation::IsMemoryMode() {
	return memory;
}

int JoinRelation::MemUsed() {
	return delegate->MemUsed();
}
