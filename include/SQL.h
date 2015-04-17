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
typedef std::pair<std::string, std::string> RelAttPair;
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
	 * Parses the sql string and puts the related meta-data into this object
	 * @param sql	The sql string that should be parsed
	 */
	virtual void Parse(const std::string &sql);

	/**
	 * Will request sql to be provided from stdin and will parse the provided sql.
	 */
	virtual void Parse();

	/**
	 * ParseWhere will take where and parse it to find OrList's into Joins or Selects based on
	 * which relation each affects. This method will assume that no Select on an OrList will
	 * affect more than one relation. The Select/Joins will be put into AndLists that will be a
	 * subset of where.
	 *
	 * Furthermore, this method requires that all relevant relations/attributes have been added to
	 * this object.
	 * @param where		The where CNF that we will parse and separate
	 * @param selects	A map that will map a relatio to it's Selection AndList
	 * @param joins		A map that will map a pair of relations to their Join AndList.
	 */
	virtual void ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins);

	/**
	 * Similar to ParseWhere, this method will go through the NameList and return a vector with the
	 * Relation/Attribute pair of the attributes. This will throw a runtime_error if an unknown
	 * relation or attribute is encountered. This method assumes that this object has been
	 * populated with all relevant data.
	 * @param list	The NameList that will be parsed
	 * @param pair	The vector of pairs that will be returned. pair.first will contain the relation
	 * 				and pair.second will contain the attribute
	 */
	virtual void ParseNameList(struct NameList *list, std::vector<RelAttPair> &pair);

};

#endif /* INCLUDE_SQL_H_ */
