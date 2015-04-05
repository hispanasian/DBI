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

	for(auto it = copyMe.lookup.begin(); it != copyMe.lookup.end(); ++it) {
		lookup[it->first] = it->second;
	}
}

Statistics::~Statistics() {
	// TODO Auto-generated destructor stub
}

void Statistics::AddRel(const char *relName, double numTuples) {
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

void Statistics::AddAtt(char *relName, char *attName, double numDistincts) {
	relations.at(relName).atts[attName] = numDistincts;

	try {
		lookup.at(attName);
	}
	catch(out_of_range &e) {
		// only update lookup if att does not exist
		lookup[attName] = relName;
	}
}

double Statistics::NumTuples(const char *relName) {
	try {
		return relations.at(relName).numTuples;
	}
	catch(out_of_range &e) {
		// Relation does not exist, return NULL.
		return -1;
	}
}

double Statistics::NumDistincts(const char *relName, const char *attName) {
	try {
		if(relations.at(relName).atts.at(attName) == -1) return relations.at(relName).numTuples;
		else return relations.at(relName).atts.at(attName);
	}
	catch(out_of_range &e) {
		return -1;
	}
}

double Statistics::ApplyAndCompute(struct AndList *parseTree, char *relNames[], int numToJoin) {
	// first verfiy that this join can take place
	if(!VerifyJoin(parseTree, relNames, numToJoin)) {
		throw std::runtime_error("Statistics::ApplyAndCompute: cannot perform join");
	}

	// now do the join
	set<string> relations;
	AndList* curr = parseTree;
	double numTuples = 0;
	while(curr != NULL) {
		relations.clear();
		// calculate which tuples are merged from this OrList
		// compute how many tuples this relation will have
		numTuples = Join(curr->left, relations);
		
		// merge these relations
		auto it = relations.begin();
		auto first = *it;
		++it;
		for(; it != relations.end(); ++it) {
			MergeSets(first, *it);
		}
		// update the number of tuples to the new value
		AddRel(first.c_str(), numTuples);
		curr = curr->rightAnd;
	}

	// the final number of tuples is the number of tuples for the entire
	// newly merged sets of relations
	return numTuples;
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{
	ApplyAndCompute(parseTree, relNames, numToJoin);
}

double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
	Statistics copy(*this);
	return copy.ApplyAndCompute(parseTree, relNames, numToJoin);
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
	bool atts = VerifyJoinAttributes(parseTree, relNames, numToJoin);
	bool sets = VerifyJoinSets(relNames, numToJoin);

	return  atts && sets;
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
			if(leftOp->code == NAME && rels.count(left[0]) == 0) return false;
			if(rightOp->code == NAME && rels.count(right[0]) == 0) return false;

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

void Statistics::MakeExpression(ComparisonOp op, std::vector<Expression*>& expressions,
	 	std::set<std::string>& relations) {
	// first determine what kind of expression this is
	int litCount = IsLiteral(op.left->code) ? 1 : 0;
	litCount += IsLiteral(op.right->code) ? 1 : 0;
	if(litCount == 2) {
		// this condition is useless for joining
		throw std::runtime_error("Statistics::MakeExpression: comparison with 2 literals");
	}
	int nameCount = IsName(op.left->code) ? 1 : 0;
	nameCount += IsName(op.right->code) ? 1 : 0;
	if(nameCount == 2) {
		// this is a binary expression
		vector<string> leftOp;
		if(!ParseOperand(op.left->value, leftOp)) {
			// could not parse the left name
			string str(op.left->value);
			throw std::runtime_error("Statistics::MakeExpression: could not parse name " + str);
		}
		vector<string> rightOp;
		if(!ParseOperand(op.right->value, rightOp)) {
			// could not parse the right name
			string str(op.right->value);
			throw std::runtime_error("Statistics::MakeExpression: could not parse name " + str);
		}
		// at this point we have a valid binary expression
		BinaryExpression* be = new BinaryExpression(*this, leftOp[0], leftOp[1], rightOp[0], rightOp[1], op.code);
		expressions.push_back(be);
		relations.insert(leftOp[0]);
		relations.insert(rightOp[0]);
	} else { 
		// this is a unary expression
		Operand* nameOp = IsName(op.left->code) ? op.left : op.right;
		Operand* litOp = IsLiteral(op.left->code) ? op.left : op.right;
		string litVal = *new string(litOp->value);
		vector<string> name;
		if(!ParseOperand(nameOp->value, name)) {
			// could not parse the name
			string str(op.right->value);
			throw std::runtime_error("Statistics::MakeExpression: could not parse name " + str);
		}
		// at this point we have a valid unary expression
		UnaryExpression* ue = new UnaryExpression(*this, name[0], name[1], litVal, op.code);
		expressions.push_back(ue);
		relations.insert(name[0]);
	}
}

bool Statistics::IsLiteral(int code) {
	return code == STRING || code == INT || code == STRING;
}

bool Statistics::IsName(int code) { return code == NAME; }

double Statistics::Join(OrList* orList, std::set<std::string>& relations) {
	OrList* curr = orList;
	vector<Expression*> expressions;
	while(curr != NULL) {
		MakeExpression(*curr->left, expressions, relations);
		curr = curr->rightOr;
	}
	CombineExpressions(expressions);
	// TODO: delete the expressions here?
	return ComputeNumTuples(expressions);
}

void Statistics::CombineExpressions(std::vector<Expression*>& expressions) {
	for(int i = 0; i < expressions.size(); ++i) {
		int j = 0;
		while(j < expressions.size()) {
			if(i < j && expressions[i]->Combine(*expressions[j])) {
				expressions.erase(expressions.begin()+j);
			} else {
				++j;
			}
		}
	}
}

double Statistics::ComputeNumTuples(std::vector<Expression*>& expressions) {
	// there's only one expression, we don't need to calculate an intersection
	if(expressions.size() == 1) {
		return expressions[0]->Tuples();
	}
	double sum = 0;
	double denominator = 1;

	for(int i = 0; i < expressions.size(); ++i) {
		sum += expressions[i]->Tuples();
		denominator *= expressions[i]->Denominator();
	}
	// subtract out the "intersection" (approximately...)
	sum -= expressions[0]->Numerator() / denominator;

	return sum;
}

void Statistics::ParseWhere(AndList *where,
		unordered_map<string, AndList*> &selects,
		unordered_map<string, unordered_map<string, AndList*> > &joins) {
	struct AndList *andList = where;
	struct OrList *orList = NULL;
	struct OrList *copyStart = NULL;
	struct OrList *copy = NULL;
	struct OrList *prev = NULL;
	struct Operand *leftOp;
	struct Operand *rightOp;
	vector<string> left;
	vector<string> right;
	set<string> affectedRels; // set of the relations that will be affected by the OrList
	int totalJoins = 0;

	bool isJoin = false;

	while(andList != NULL) {
		int joinCount = 0;
		orList = andList->left;
		copy = new OrList();
		copyStart = copy;

		while(orList != NULL) {
			// First, make a copy of current CoparisonOp in the OrList
			copy->left = orList->left;
			copy->rightOr = new OrList();
			prev = copy;
			copy = copy->rightOr;

			// Verify that the relation/attribute is valid
			leftOp = orList->left->left;
			rightOp = orList->left->right;
			if( (leftOp->code == NAME && ParseOperand(leftOp->value, left)) == false || // left operand
					(rightOp->code == NAME && ParseOperand(rightOp->value, right) == false)) // right operand
				throw runtime_error("Relation or attribute does not exist");

			// First, let's check to see if we have a join
			if(leftOp->code == NAME && rightOp->code == NAME) {
				// update the affected relations
				affectedRels.insert(left[0]);
				affectedRels.insert(right[0]);

				// Variable upkeep
				isJoin = true;
				++joinCount;
				++totalJoins;
			}
			// We are parsing a Select
			else {
				if(leftOp->code == NAME) affectedRels.insert(left[0]);
				else affectedRels.insert(right[0]);
			}

			// Prepare for the next Or
			orList = orList->rightOr;
		}
		// Remove extraneous Or
		copy = prev;
		delete copy->rightOr;
		copy->rightOr = NULL;

		// Prepare the AndList
		AndList *op = new AndList();
		op->rightAnd = NULL;
		op->left = copyStart;


		// Create the join if there are no errors
		if(isJoin) {
			// Can't join on more than one relation/attribute pair
			if(joinCount > 1) throw runtime_error("We can only join on a single pair of attributes in an OrList");
			if(affectedRels.size() != 2) throw runtime_error("We can't join on the same relation");

			// Attempt to add the Join. However, if There is already a join on this pair of
			// relations, throw an error
			try {
				joins[left[0]].at(right[0]);
				joins[right[0]].at(left[0]);

				throw runtime_error("Cannot Join on a pair of relations more than once");
			} catch(out_of_range &e) {
				joins[left[0]].insert(OrPair(right[0], op));
				joins[right[0]].insert(OrPair(left[0], op));
			}

		}
		// Create/Append to the select if there are no errors
		else {
			if(affectedRels.size() != 1) throw runtime_error("The OrList cannot affect more than one relation");
			string rel = *(affectedRels.begin()); // Get the name of the affected Relation
			try {
				// The current AndList associated with the select
				AndList *temp = selects.at(rel);

				// Append to existing AndList
				while(temp->rightAnd != NULL) {
					temp = temp->rightAnd;
				}
				temp->rightAnd = op;

			} catch(out_of_range &e) {
				// The relation does not have an AndList yet
				selects.insert(OrPair(rel, op));
			}
		}

		// Variable upkeep
		joinCount = 0;
		isJoin = false;
		affectedRels.clear();
		andList = andList->rightAnd;
	}
	// Check to see that all the relations have been joined and that there are no excess joins
	if((relations.size() - totalJoins) != 1) throw runtime_error("The AndList contains an unexpected number of joins");
}

void Statistics::ParseNameList(NameList *list, vector<RelAttPair> &pair) {

}
