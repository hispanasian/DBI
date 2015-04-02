/*
 * Statistics.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

using namespace std;

#include "Statistics.h"

Statistics::Statistics(): relations(myRelations) {

}

Statistics::Statistics(unordered_map<string, StatPair> &_relations): relations(_relations) {

}

Statistics::~Statistics() {
	// TODO Auto-generated destructor stub
}

void Statistics::AddRel(char *relName, int numTuples) {
	relations[relName].numTuples = numTuples;
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts) {
	relations.at(relName).atts[attName] = numDistincts;
}

int Statistics::NumTuples(char *relName) {
	try {
		return relations.at(relName).numTuples;
	}
	catch(out_of_range &e) {
		// Relation does not exist, return NULL.
		return NULL;
	}
}

int Statistics::NumDistincts(char *relName, char *attName) {
	return NULL;
}
