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

Statistics::Statistics(const Statistics &copyMe): relations(myRelations) {
	string rel;
	string att;
	int val;
	StatPair temp;

	for(auto it = copyMe.relations.begin(); it != copyMe.relations.end(); ++it) {
		rel = it->first;
		temp = it->second;
		relations[rel].numTuples = temp.numTuples; // Copy the relation
		for(auto a_it = temp.atts.begin(); a_it != temp.atts.end(); ++a_it) {
			att = a_it->first;
			relations[rel].atts[a_it->first] =  a_it->second; // Copy the attribute
		}
	}
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
	try {
		return relations.at(relName).atts.at(attName);
	}
	catch(out_of_range &e) {
		return NULL;
	}
}
