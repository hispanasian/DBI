/*
 * Statistics.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

using namespace std;

#include "Statistics.h"
#include <iostream>

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

void Statistics::Read(char *fromWhere) {
	RawFile file;
	Read(fromWhere, file);
}

void Statistics::Read(char *fromWhere, RawFile &file) {
	string line;
	string rel;
	string val;
	int equals;

	file.Open(fromWhere);
	file.ReadLine(&line); // The first line should be Relations

	// First, read in the relations
	while(file.ReadLine(&line) == true && line.compare("Attributes") != 0) {
		equals = line.find_first_of("=");
		if(equals != string::npos) { // ignore incorrectly formatted relations
			rel = line.substr(0, equals);
			val = line.substr(equals + 1, line.length() + 1);
			relations[rel].numTuples = stoi(val);
		}
	}

	string att;
	int dot;
	// Now read in the attributes
	while(file.ReadLine(&line) == true) {
		equals = line.find_first_of("=");
		dot = line.find_first_of(".");
		if(equals != string::npos && dot != string::npos) { // ignore incorrectly formatted attributes
			rel = line.substr(0, dot);
			att = line.substr(dot+1, equals - dot - 1);
			val = line.substr(equals + 1, line.length() + 1);
			relations.at(rel).atts[att] = stoi(val);
		}
	}

	file.Close();
}

void Statistics::Write(char *fromWhere) {
	RawFile file;
	Write(fromWhere, file);
}

void Statistics::Write(char *fromWhere, RawFile &file) {
	file.Open(fromWhere);

	string rel;
	string val;
	// First, write the Relations
	file.Append("Relations\n");
	for(auto it = relations.begin(); it != relations.end(); ++it) {
		rel = it->first;
		val = to_string(it->second.numTuples);
		file.Append(rel + "=" + val + "\n");
	}

	string att;
	// Second, write the Attributes
	file.Append("Attributes\n");
	for(auto it = relations.begin(); it != relations.end(); ++it) {
		rel = it->first;
		val = to_string(it->second.numTuples);
		for(auto a_it = relations.at(rel).atts.begin(); a_it != relations.at(rel).atts.end(); ++a_it) {
			att = a_it->first;
			val = to_string(a_it->second);
			file.Append(rel + "." + att + "=" + val + "\n");
		}
	}

	file.Close();
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
