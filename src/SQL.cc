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
		create(NULL), selectDistinct(0), aggregateDistinct(0), relationSize(0), output(-1),
		instruction(-1), file(NULL), table(NULL) {}

SQL::SQL(const Statistics &_stat): stat(_stat), function(NULL), relations(NULL), where(NULL),
		groupAtts(NULL), selectAtts(NULL), create(NULL), selectDistinct(0), aggregateDistinct(0),
		relationSize(0), output(-1), instruction(-1), file(NULL), table(NULL)  {}

SQL::SQL(const Statistics &_stat, int _relationSize): stat(_stat), function(NULL), relations(NULL),
		where(NULL), groupAtts(NULL), selectAtts(NULL), create(NULL), selectDistinct(0),
		aggregateDistinct(0), relationSize(_relationSize), output(-1), instruction(-1),
		file(NULL), table(NULL) {}

SQL::SQL(const SQL &copyMe): function(copyMe.function), relations(copyMe.relations),
		where(copyMe.where), groupAtts(copyMe.groupAtts), selectAtts(copyMe.selectAtts),
		create(createData), selectDistinct(copyMe.selectDistinct),
		aggregateDistinct(copyMe.aggregateDistinct), relationSize(copyMe.relationSize),
		output(outType), instruction(command), file(refFile), table(refTable)  {}

SQL::~SQL() {
	// TODO Auto-generated destructor stub
}

const struct FuncOperator* SQL::Function() const {
	return function;
}
const Statistics& SQL::GetStatistics() const {
	return stat;
}

string SQL::GetSQLStatement() const {
	return sql;
}

SQL_Command  SQL::Parse(const string &sql) {
	this->sql = sql;
	yysql_scan_string(this->sql.c_str());
	return Parse();
}

SQL_Command SQL::Parse() {
	yysqlparse();

	// Copy the structures
	function = finalFunction;
	relations = tables;
	where = boolean;
	groupAtts = groupingAtts;
	selectAtts = attsToSelect;
	create = createData;
	selectDistinct = distinctAtts;
	aggregateDistinct = distinctFunc;
	output = outType;
	instruction = command;
	file = refFile;
	table = refTable;

	// Make copy of aliases
	vector<RelAliasPair> aliases;
	GetTables(aliases);
	for(int i = 0; i < aliases.size(); i++) {
		stat.CopyRel(aliases[i].Relation().c_str(), aliases[i].Alias().c_str());
	}
	relationSize = aliases.size();

	switch (instruction) {
	case CREATE: return Create_Table;
	case INSERT_INTO: return Insert_Into;
	case DROP: return Drop_Table;
	case OUTPUT_SET: return Set_Output;
	case SELECT_TABLE: return Select;
	case QUIT_SQL: return Quit;
	default: return Unknown;
	}
}

void SQL::GetWhere(SelectMap &selects, JoinMap &joins) const {
	ParseWhere(where, selects, joins);
}

void SQL::GetGroup(std::vector<RelAttPair> &pairs) const {
	ParseNameList(groupAtts, pairs);
}

void SQL::GetSelect(vector<RelAttPair> &pairs) const {
	ParseNameList(selectAtts, pairs);
}

void SQL::GetTables(std::vector<RelAliasPair> &pairs) const {
	ParseTableList(relations, pairs);
}

void SQL::GetFunctionAttributes(vector<RelAttPair> &pairs) const {
	if(function != NULL) ParseFuncOperator(function, pairs);
}

bool SQL::DistinctAggregate() const { return aggregateDistinct; }

bool SQL::DistinctSelect() const { return selectDistinct; }

bool SQL::ParseOperand(string operand, vector<string> &out) const {
	return stat.ParseOperand(operand, out);
}

void SQL::ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins) const {
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

void SQL::ParseNameList(NameList *names, vector<RelAttPair> &pair) const {
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

void SQL::ParseFuncOperator(FuncOperator *func, vector<RelAttPair> &pair) const {
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

void SQL::ParseTableList(TableList *list, vector<RelAliasPair> &pairs) const {
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

DB_Type SQL::GetCreateTable(vector<AttTypePair> &atts, vector<string> &order, string &tableName) const {
	// First, let's verify the relation/table does not exist
//	tableName.erase();
//	tableName.append(string(table));
	tableName = string(table);
	if(stat.NumTuples(tableName.c_str()) != -1)
		throw invalid_argument("A table with the provided name already exists");

	// Now, builds the table
	atts.clear();
	order.clear();
	ParseCreateTable(create, atts, order);

	// Lastly, return the type
	if(create->type == SORTED_DB) return Sorted;
	else if(create->type == HEAP_DB) return Heap;
	else throw invalid_argument("Unknown Table type found");
}

void SQL::ParseCreateTable(const CreateTable *table, vector<AttTypePair> &atts,
		vector<string> &order) const {
	// First, lets build atts
	stack<AttTypePair> tempAtts;
	AttDesc *att = table->atts;
	while(att != NULL) {
		tempAtts.push(AttTypePair(att->name, att->type));
		att = att->next;
	}

	while(tempAtts.size() > 0) {
		atts.push_back(tempAtts.top());
		tempAtts.pop();
	}

	// Now build/verify order
	stack<string> tempOrder;
	NameList *name = table->sort;
	bool found = false;
	while(name != NULL) {
		tempOrder.push(string(name->name));

		// Verify
		found = false;
		for(int i = 0; i < atts.size() && !found; i++) {
			if(atts[i].Attribute().compare(tempOrder.top()) == 0 ) found = true;
		}
		if(!found) throw invalid_argument("Cannot sort on non-existing attribute");
		name = name->next;
	}

	while(tempOrder.size() > 0) {
		order.push_back(tempOrder.top());
		tempOrder.pop();
	}
}
