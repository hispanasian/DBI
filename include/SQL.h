/*
 * SQL.h
 *
 *  Created on: Apr 16, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SQL_H_
#define INCLUDE_SQL_H_

#include "Statistics.h"

typedef std::pair<std::string, AndList*> OrPair;
typedef std::pair<std::string, std::string> StringPair;
typedef std::unordered_map<std::string, AndList*> SelectMap;
typedef std::unordered_map<std::string, std::unordered_map<std::string, AndList*> > JoinMap;

extern "C" {
	int yysqlparse(void);   // defined in y.tab.c
	int yysqlfuncparse(void);   // defined in yyfunc.tab.c
	void init_lexical_parser_sql (char *); // defined in lex.yy.c (from Lexer.l)
	void close_lexical_parser_sql (); // defined in lex.yy.c
	void init_lexical_parser_func (char *); // defined in lex.yyfunc.c (from Lexerfunc.l)
	void close_lexical_parser_func (); // defined in lex.yyfunc.c
	struct YY_BUFFER_STATE *yysql_scan_string(const char*);
}

// these data structures hold the result of the parsing
extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
extern struct TableList *tables; // the list of tables and aliases in the query
extern struct AndList *boolean; // the predicate in the WHERE clause
extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query
extern int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query

class RelAttPair{
protected:
	std::string relation;
	std::string attribute;
public:
	RelAttPair(std::string &_relation, std::string &_attribute) {
		relation = _relation;
		attribute = _attribute;
	}

	RelAttPair(const RelAttPair &copyMe) {
		relation = copyMe.relation;
		attribute = copyMe.attribute;
	}

	RelAttPair(const StringPair &copyMe) {
		relation = copyMe.first;
		attribute = copyMe.second;
	}

	std::string Relation() { return relation; }
	std::string Attribute() { return attribute; }
};

class RelAliasPair{
protected:
	std::string relation;
	std::string alias;
public:
	RelAliasPair(std::string &_relation, std::string &_alias) {
		relation = _relation;
		alias = _alias;
	}

	RelAliasPair(const RelAliasPair &copyMe) {
		relation = copyMe.relation;
		alias = copyMe.alias;
	}

	RelAliasPair(const StringPair &copyMe) {
		relation = copyMe.first;
		alias = copyMe.second;
	}

	std::string Relation() { return relation; }
	std::string Alias() { return alias; }
};

class SQL {
protected:
	Statistics stat;
	std::string sql;

	// these data structures hold the result of the parsing
	struct FuncOperator *function; // the aggregate function (NULL if no agg)
	struct TableList *relations; // the list of tables and aliases in the query
	struct AndList *where; // the predicate in the WHERE clause
	struct NameList *groupAtts; // grouping atts (NULL if no grouping)
	struct NameList *selectAtts; // the set of attributes in the SELECT (NULL if no such atts)
	int selectDistinct; // 1 if there is a DISTINCT in a non-aggregate query
	int aggregateDistinct;  // 1 if there is a DISTINCT in an aggregate query

	/**
	 * Parses the operand and puts the relation and attribute in out. The format of the operand
	 * should be one of the following:
	 * 1. attribute
	 * 2. relation.attribute
	 *
	 * The ouput will be as follows:
	 * out[0] = relation
	 * out[1] = attribute
	 *
	 * This method will return false if the attribute in the operand does not exist.
	 * @param operand	The operand being parsed
	 * @param out		The vector that will hold the resulting relation and attribute
	 * @return			True if the operand can be parsed and the attribute can be found
	 */
	virtual bool ParseOperand(std::string operand, std::vector<std::string> &out);

public:

	SQL();

	/**
	 * Creates SQL with a copy of stat
	 */
	SQL(const Statistics &stat);
	virtual ~SQL();

	/**
	 * Parses the sql string and puts the related meta-data into this object. This method will
	 * correctly update the Statistics object to reflect aliased relations
	 */
	virtual void Parse(const std::string &sql);

	/**
	 * Will request sql to be provided from stdin and will parse the provided sql. This method will
	 * correctly update the Statistics object to reflect aliased relations
	 */
	virtual void Parse();

	/**
	 * Returns a SelectMap and JoinMap that represents the Selects and Joins that are specified in
	 * the WHERE clause of this SQL statement.
	 * @param selects	A map of the selects (relation->AndList)
	 * @param joins		A map of the joins (relation1->relation2->AndList)
	 */
	virtual void GetWhere(SelectMap &selects, JoinMap &joins);

	/**
	 * Returns a vector of the Attributes in the GroupBy clause
	 * @param pairs	The vector that will hold the Relation/Attribute pairs
	 */
	virtual void GetGroup(std::vector<RelAttPair> &pairs);

	/**
	 * Returns a vector of the Attributes in the Select clause (excluding the aggregated attributes)
	 * @param pairs	The vector that will hold the Relation/Attribute pairs
	 */
	virtual void GetSelect(std::vector<RelAttPair> &pairs);

	/**
	 * Returns a vector of the Relations (and their aliases) referenced in the FROM clause
	 * @param pairs	The vector that will hold the Relation/Alias pairs
	 */
	virtual void GetTables(std::vector<RelAliasPair> &pairs);

	/**
	 * Returns the attributes that are referenced in the aggregate
	 * @param pair	The vector that will hold the referenced relation/attribute pairs
	 */
	virtual void GetFunctionAttributes(std::vector<RelAttPair> &pair);

	/**
	 * Returns true if there is a distinct in the aggregate function
	 * @param true if there is a distinct in the aggregate function
	 */
	virtual bool DistinctAggregate();

	/**
	 * Returns true if there is a distinct in the selection
	 * @param true if there is a distinct in the selction
	 */
	virtual bool DistinctSelect();

	/**
	 * ParseWhere will take where and parse it to find OrList's into Joins or Selects based on
	 * which relation each affects. This method will assume that no Select on an OrList will
	 * affect more than one relation. The Select/Joins will be put into AndLists that will be a
	 * subset of where.
	 *
	 * Furthermore, this method requires that all relevant relations/attributes have been added to
	 * this objects Statistics object.
	 * @param where		The where CNF that we will parse and separate
	 * @param selects	A map that will map a relation to it's Selection AndList
	 * @param joins		A map that will map a pair of relations to their Join AndList.
	 */
	virtual void ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins);

	/**
	 * Similar to ParseWhere, this method will go through the NameList and return a vector with the
	 * Relation/Attribute pair of the attributes. This will throw a runtime_error if an unknown
	 * relation or attribute is encountered. This method assumes that this objects Statistics
	 *  object has been populated with all relevant data.
	 * @param list	The NameList that will be parsed
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseNameList(struct NameList *list, std::vector<RelAttPair> &pair);

	/**
	 * Parses a FuncOperator and returns a vector of RelAttPair representing the attributes that
	 * are contained by func.
	 * @param func	The FuncOperator being parsed
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseFuncOperator(FuncOperator *func, std::vector<RelAttPair> &pair);

	/*
	 * Parses a TableList and returns a vector of RelAlias pair representing the aliases that
	 * correspond to each relation
	 * @param list	The TableList that contains the map of each alias
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseTableList(TableList *list, std::vector<RelAliasPair> &pairs);

};

#endif /* INCLUDE_SQL_H_ */
