/*
 * JoinRelation.cc
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#include "JoinRelation.h"

JoinRelation::JoinRelation(int memLimit): Relation(memLimit) {
	memory = true;

}

JoinRelation::~JoinRelation() {
	// TODO Auto-generated destructor stub
}

bool JoinRelation::Add(Record *rec) {
	return true;
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
