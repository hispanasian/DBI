/*
 * Statistics.cc
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

using namespace std;

#include "Statistics.h"
#include <iostream>

Statistics::Statistics(): relations(myRelations), lookup(myLookup) {}

Statistics::Statistics(std::unordered_map<std::string, StatData> &_relations,
		std::unordered_map<std::string, std::string> &_lookup):
				relations(_relations), lookup(_lookup) {
}

Statistics::Statistics(const Statistics &copyMe): relations(myRelations), lookup(myLookup) {
	string rel;
	string att;
	int val;
	StatData temp;

	for(auto it = copyMe.relations.begin(); it != copyMe.relations.end(); ++it) {
		rel = it->first;
		temp = it->second;
		relations[rel].numTuples = temp.numTuples; // Copy the relation
		for(auto a_it = temp.atts.begin(); a_it != temp.atts.end(); ++a_it) {
			att = a_it->first;
			relations[rel].atts[a_it->first] =  a_it->second; // Copy the attribute
		}
		for(auto s_it = temp.set.begin(); s_it != temp.set.end(); ++s_it) {
			relations[rel].set.insert(*s_it);
		}
	}
}

Statistics::~Statistics() {
	// TODO Auto-generated destructor stub
}

void Statistics::AddRel(char *relName, int numTuples) {
	try {
		set<string> &temp = relations.at(relName).set;

		// Update the numTuples of every relation in the set
		for(auto it = temp.begin(); it != temp.end(); ++it) {
			UpdateRel((*it).c_str(), numTuples);
		}
	}
	catch(out_of_range &e) {
		// Relation does not exist, there is no set to update so just add relName to its own set
		relations[relName].numTuples = numTuples;
		relations[relName].set.insert(relName);
	}
}

void Statistics::UpdateRel(const char *relName, int numTuples) {
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
			relations[rel].set.insert(rel);
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
			lookup[att] = rel;
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

void Statistics::CopyRel(char *oldName, char *newName) {
	relations[newName].numTuples = relations.at(oldName).numTuples;

	for(auto it = relations.at(oldName).atts.begin(); it != relations.at(oldName).atts.end(); ++it) {
		relations[newName].atts[it->first] = it->second;
	}

	// Remember to add itself to its own set
	relations[newName].set.insert(newName);
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts) {
	relations.at(relName).atts[attName] = numDistincts;

	try {
		lookup.at(attName);
	}
	catch(out_of_range &e) {
		// only update lookup if att does not exist
		lookup[attName] = relName;
	}
}

int Statistics::NumTuples(const char *relName) {
	try {
		return relations.at(relName).numTuples;
	}
	catch(out_of_range &e) {
		// Relation does not exist, return NULL.
		return -1;
	}
}

int Statistics::NumDistincts(const char *relName, const char *attName) {
	try {
		return relations.at(relName).atts.at(attName);
	}
	catch(out_of_range &e) {
		return -1;
	}
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{

}

double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{

}

void Statistics::MergeSets(std::string rel1, std::string rel2) {
	set<string> temp1 = GetSet(rel1);
	set<string> temp2 = GetSet(rel2);

	// First, update all the sets containing rel1
	for(auto it1 = temp1.begin(); it1 != temp1.end(); ++it1) {
		for(auto it2 = temp2.begin(); it2 != temp2.end(); ++it2) {
			relations[*it1].set.insert(*it2);
		}
	}

	// Now, update all the sets containing rel2
	for(auto it1 = temp2.begin(); it1 != temp2.end(); ++it1) {
		for(auto it2 = temp1.begin(); it2 != temp1.end(); ++it2) {
			relations[*it1].set.insert(*it2);
		}
	}
}

bool Statistics::VerifyJoin(struct AndList *parseTree, char **relNames, int numToJoin) {
	return VerifyJoinAttributes(parseTree, relNames, numToJoin) && VerifyJoinSets(relNames, numToJoin);
}

bool Statistics::VerifyJoinAttributes(struct AndList *parseTree, char **relNames, int numToJoin) {
	struct AndList *andList = parseTree;
	struct OrList *orList = NULL;
	struct Operand *leftOp;
	struct Operand *rightOp;
	vector<string> left;
	vector<string> right;
	set<string> rels;

	// Populate rels with the relations in relNames
	for(int i = 0; i < numToJoin; ++i) {
		rels.insert(relNames[i]);
	}

	while(andList != NULL) {
		orList = andList->left;
		while(orList != NULL) {
			// First, verify that the relation/attribute is valid
			leftOp = orList->left->left;
			rightOp = orList->left->right;
			if( (leftOp->code == NAME && ParseOperand(leftOp->value, left)) == false || // left operand
					(rightOp->code == NAME && ParseOperand(rightOp->value, right) == false)) // right operand
				return false;

			// Next, check to see if the relations exist in the rels.
			if(rels.count(left[0]) == 0) return false;
			if(rels.count(right[0]) == 0) return false;

			orList = orList->rightOr;
		}
		andList = andList->rightAnd;
	}
	return true;
}

bool Statistics::VerifyJoinSets(char **relNames, int numToJoin) {
	// Create a set that will hold every relation from every set that contains any relation in
	// relNames
	set<string> superset;
	set<string> temp;
	for(int i = 0; i < numToJoin; ++i) {
		temp = GetSet(relNames[i]);
		for(auto it = temp.begin(); it != temp.end(); ++it) {
			superset.insert(*it);
		}
	}

	// Now, remove every in relNames from superset. If there are any remaining elements, the join
	// is invalid
	for(int i = 0; i < numToJoin; ++i) {
		superset.erase(relNames[i]);
	}
	return superset.empty();
}

bool Statistics::ParseOperand(string operand, vector<string> &out) {
	string relation;
	string attribute;
	out.clear();

	if(operand.find(".") == string::npos) {
		// No relation provided
		attribute = operand;
		relation = RelLookup(attribute);
	}
	else {
		// relation provided
		attribute = operand.substr(operand.find(".") + 1, operand.length() - operand.find("."));
		relation = operand.substr(0, operand.find("."));
	}

	// Now to verify that the attributes and relations actually exist.
	if(NumDistincts(relation.c_str(), attribute.c_str()) == -1) {
		return false;
	}
	out.push_back(relation);
	out.push_back(attribute);

	return true;
}

string Statistics::RelLookup(string att) {
	try {
		return lookup.at(att);
	}
	catch(out_of_range &e) {
		return "";
	}
}

set<string> Statistics::GetSet(string rel) {
	return relations.at(rel).set;
}
