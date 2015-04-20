/*
 * SQL.cc
 *
 *  Created on: Apr 16, 2015
 *      Author: cvasquez
 */

#include "SQL.h"
#include <stack>
#include <stdio.h>
#include <iostream>

using namespace std;

SQL::SQL(): function(NULL), relations(NULL), where(NULL), groupAtts(NULL), selectAtts(NULL),
		selectDistinct(0), aggregateDistinct(0), relationSize(0) {}

SQL::SQL(const Statistics &_stat): stat(_stat), function(NULL), relations(NULL), where(NULL),
		groupAtts(NULL), selectAtts(NULL), selectDistinct(0), aggregateDistinct(0), relationSize(0) {}

SQL::SQL(const Statistics &_stat, int _relationSize): stat(_stat), function(NULL), relations(NULL),
		where(NULL), groupAtts(NULL), selectAtts(NULL), selectDistinct(0), aggregateDistinct(0),
		relationSize(_relationSize) {}

SQL::~SQL() {
	// TODO Auto-generated destructor stub
}

const struct FuncOperator* SQL::Function() const {
	return function;
}
const Statistics& SQL::GetStatistics() const {
	return stat;
}


void SQL::Parse(const string &sql) {
	this->sql = sql;
	yysql_scan_string(this->sql.c_str());
	Parse();
}

void SQL::Parse() {
	yysqlparse();

	// Copy the structures
	function = finalFunction;
	relations = tables;
	where = boolean;
	groupAtts = groupingAtts;
	selectAtts = attsToSelect;
	selectDistinct = distinctAtts;
	aggregateDistinct = distinctFunc;

	// Make copy of aliases
	vector<RelAliasPair> aliases;
	GetTables(aliases);
	for(int i = 0; i < aliases.size(); i++) {
		stat.CopyRel(aliases[i].Relation().c_str(), aliases[i].Alias().c_str());
	}
	relationSize = aliases.size();
}

void SQL::GetWhere(SelectMap &selects, JoinMap &joins) {
	ParseWhere(where, selects, joins);
}

void SQL::GetGroup(std::vector<RelAttPair> &pairs) {
	ParseNameList(groupAtts, pairs);
}

void SQL::GetSelect(vector<RelAttPair> &pairs) {
	ParseNameList(selectAtts, pairs);
}

void SQL::GetTables(std::vector<RelAliasPair> &pairs) {
	ParseTableList(relations, pairs);
}

void SQL::GetFunctionAttributes(vector<RelAttPair> &pairs) {
	if(function != NULL) ParseFuncOperator(function, pairs);
}

bool SQL::DistinctAggregate() { return aggregateDistinct; }

bool SQL::DistinctSelect() { return selectDistinct; }

bool SQL::ParseOperand(string operand, vector<string> &out) {
	return stat.ParseOperand(operand, out);
}

void SQL::ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins) {
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
	if((relationSize - totalJoins) != 1) throw runtime_error("The AndList contains an unexpected number of joins");

	// Check that every relation has some kind of AndList. If not, set an empty AndList
	vector<RelAliasPair> rels;
	GetTables(rels);

	for(auto it = rels.begin(); it != rels.end(); it++) {
		string alias = (*it).Alias();
		try {
			// check to see if the relation got an andlist
			selects.at(alias);
		}
		catch (out_of_range &e){
			selects.insert( {alias, new AndList{NULL, NULL}} );
		}
	}
}

void SQL::ParseNameList(NameList *names, vector<RelAttPair> &pair) {
	stack<RelAttPair> temp;
	vector<string> name;
	while(names != NULL) {
		if(ParseOperand(names->name, name)) temp.push(make_pair(name[0], name[1]));
		else throw runtime_error("Error: Relation or attribute does not exist. (Statistics::ParseNameList)");
		names = names->next;
	}

	// Now, return the correctly ordered pairs
	while(temp.size() > 0) {
		pair.push_back(temp.top());
		temp.pop();
	}
}

void SQL::ParseFuncOperator(FuncOperator *func, vector<RelAttPair> &pair) {
	if(func != NULL) {
		// First, parse left child
		ParseFuncOperator(func->leftOperator, pair);

		// Next, check root.
		struct FuncOperand *op = func->leftOperand;
		if(op != NULL && op->code == NAME) {
			vector<string> name;
			if(ParseOperand(op->value, name)) pair.push_back(make_pair(name[0], name[1]));
			else throw runtime_error("Error: Relation or attribute does not exist. (Statistics::ParseFuncOperator)");
		}

		// Finally, check right child
		ParseFuncOperator(func->right, pair);
	}
}

void SQL::ParseTableList(TableList *list, vector<RelAliasPair> &pairs) {
	stack<RelAliasPair> temp;
	while(list != NULL) {
		if(stat.NumTuples(list->tableName) != -1)
			temp.push(make_pair(string(list->tableName), string(list->aliasAs)));
		else throw runtime_error("Error: Relation does not exist. (Statistics::ParseTableList)");
		list = list->next;
	}

	// Now, return the correctly ordered pairs
	while(temp.size() > 0) {
		pairs.push_back(temp.top());
		temp.pop();
	}
}
