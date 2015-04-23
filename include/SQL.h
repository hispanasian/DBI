/*
 * SQL.h
 *
 *  Created on: Apr 16, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SQL_H_
#define INCLUDE_SQL_H_

#include "Statistics.h"
#include "Defs.h"
#include "ParseTree.h"
#include <iostream>

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
extern struct NameList *attsToCreate; // the set of attributes in the SELECT (NULL if no such atts)
extern struct CreateTable* createData; // data associated with creating a Table
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query
extern int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
extern int command; // Says whether it is a create table, insert, drop table, set output, or select
extern int outType; // The type of the output
extern char *refFile; // a referenced file
extern char *refTable; // a referenced table

class RelAttPair{
protected:
	std::string relation;
	std::string attribute;
public:
	RelAttPair(std::string &_relation, std::string &_attribute) {
		relation = _relation;
		attribute = _attribute;
	}

	RelAttPair(const char *_relation, const char* _attribute) {
		relation = std::string(_relation);
		attribute = std::string(_attribute);
	}

	RelAttPair(const RelAttPair &copyMe) {
		relation = copyMe.relation;
		attribute = copyMe.attribute;
	}

	RelAttPair(const StringPair &copyMe) {
		relation = copyMe.first;
		attribute = copyMe.second;
	}

	void Copy(const RelAttPair &copyMe) {
		relation = copyMe.relation;
		attribute = copyMe.attribute;
	}

	std::string Relation() const { return relation; }
	std::string Attribute() const { return attribute; }
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

	RelAliasPair(const char* _relation, const char* _alias) {
		relation = std::string(_relation);
		alias = std::string(_alias);
	}

	RelAliasPair(const RelAliasPair &copyMe) {
		relation = copyMe.relation;
		alias = copyMe.alias;
	}

	RelAliasPair(const StringPair &copyMe) {
		relation = copyMe.first;
		alias = copyMe.second;
	}

	void Copy(const RelAliasPair &copyMe) {
		relation = copyMe.relation;
		alias = copyMe.alias;
	}

	std::string Relation() const { return relation; }
	std::string Alias() const { return alias; }
};

class AttTypePair{
protected:
	std::string att;
	Type type;
public:
	AttTypePair(const std::string &_att, Type &_type): att(_att), type(_type) {}

	AttTypePair(const char* _att, int _type): att(std::string(_att)) {
		if(_type == INT) type = Int;
		else if(_type == DOUBLE) type = Double;
		else if (_type == STRING) type = String;
		else throw std::invalid_argument("Unknown type found");
	}

	AttTypePair(const AttTypePair &copyMe) {
		att = copyMe.att;
		type = copyMe.type;
	}

	std::string Attribute() const { return att; }
	Type GetType() const { return type; }
	std::string TypeString() const {
		if(type == Int) return std::string("Int");
		else if(type == Double) return std::string("Double");
		else return std::string("String");
	}
};

class SQL {
protected:
	Statistics stat;
	std::string sql;
	int relationSize;

	// these data structures hold the result of the parsing
	struct FuncOperator *function; // the aggregate function (NULL if no agg)
	struct TableList *relations; // the list of tables and aliases in the query
	struct AndList *where; // the predicate in the WHERE clause
	struct NameList *groupAtts; // grouping atts (NULL if no grouping)
	struct NameList *selectAtts; // the set of attributes in the SELECT (NULL if no such atts)
	struct CreateTable *create; // The data associated with creating a table
	int selectDistinct; // 1 if there is a DISTINCT in a non-aggregate query
	int aggregateDistinct;  // 1 if there is a DISTINCT in an aggregate query
	int output; // The type of output selected
	int instruction; // The instruction being executed (Select, Insrt, etc.)
	char *file; // The file referenced by the instruction
	char *table; // The table referenced by the instruction


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
	virtual bool ParseOperand(std::string operand, std::vector<std::string> &out) const;

public:

	SQL();

	/**
	 * Creates SQL with a copy of stat
	 */
	SQL(const Statistics &_stat);
	SQL(const Statistics &_stat, int _relationSize); // strictly for testing
	SQL(const SQL &copyMe);
	virtual ~SQL();

	/**
	 * Returns the FuncOperator produced by this SQL
	 * @param	Returns the FuncOperator produced by this SQL or NULL if there is none
	 */
	virtual const struct FuncOperator* Function() const;

	/**
	 * Returns the Statistics object used by this SQL object.
	 */
	virtual const Statistics& GetStatistics() const;

	/**
	 * Returns the SQL statement
	 * @return	The SQL statement
	 */
	virtual std::string GetSQLStatement() const;

	/**
	 * Parses the sql string and puts the related meta-data into this object. This method will
	 * correctly update the Statistics object to reflect aliased relations
	 * @return	The type of command that was parsed
	 */
	SQL_Command Parse(const std::string &sql);

	/**
	 * Will request sql to be provided from stdin and will parse the provided sql. This method will
	 * correctly update the Statistics object to reflect aliased relations
	 * @return	The type of command that was parsed
	 */
	SQL_Command Parse();

	/**
	 * Returns a SelectMap and JoinMap that represents the Selects and Joins that are specified in
	 * the WHERE clause of this SQL statement.
	 * @param selects	A map of the selects (relation->AndList)
	 * @param joins		A map of the joins (relation1->relation2->AndList)
	 */
	virtual void GetWhere(SelectMap &selects, JoinMap &joins) const;

	/**
	 * Returns a vector of the Attributes in the GroupBy clause
	 * @param pairs	The vector that will hold the Relation/Attribute pairs
	 */
	virtual void GetGroup(std::vector<RelAttPair> &pairs) const;

	/**
	 * Returns a vector of the Attributes in the Select clause (excluding the aggregated attributes)
	 * @param pairs	The vector that will hold the Relation/Attribute pairs
	 */
	virtual void GetSelect(std::vector<RelAttPair> &pairs) const;

	/**
	 * Returns a vector of the Relations (and their aliases) referenced in the FROM clause
	 * @param pairs	The vector that will hold the Relation/Alias pairs
	 */
	virtual void GetTables(std::vector<RelAliasPair> &pairs) const;

	/**
	 * Returns the attributes that are referenced in the aggregate
	 * @param pair	The vector that will hold the referenced relation/attribute pairs
	 */
	virtual void GetFunctionAttributes(std::vector<RelAttPair> &pair) const;

	/**
	 * Puts the parsed CREATE TABLE statement into atts and order. An invalid_input exception will
	 * be thrown if the DB_Type is Sorted and there is no order.
	 * @param atts		The attribute/types that should be created
	 * @param order		The order (if there is any)
	 * @param tableName	The name of the relation that should be created (is inserted into this var)
	 * @return	The type of table that should be created
	 */
	DB_Type GetCreateTable(std::vector<AttTypePair> &atts, std::vector<std::string> &order,
			std::string &tableName) const;

	/**
	 * Gets the file that will be used to bulkload data into table
	 * @param file	The file which contains the data
	 * @param table	The table that will be populated
	 */
	void GetInsertInto(std::string &file, std::string &table) const;

	/**
	 * Returns the name of the Table that should be dropped
	 * @param the name of the Table that should be dropped
	 */
	std::string GetDropTable() const;

	/**
	 * Gets the parameters of the SET OUTPUT command. If File is returned, then the path will be
	 * put into file, else file will be empty.
	 * @param file	The path to the file which will hold the output
	 * @return		The type of output (stdout, a file, or none)
	 */
	Output_Type GetSetOutput(std::string &file) const;

	/**
	 * Returns true if there is a distinct in the aggregate function
	 * @param true if there is a distinct in the aggregate function
	 */
	virtual bool DistinctAggregate() const;

	/**
	 * Returns true if there is a distinct in the selection
	 * @param true if there is a distinct in the selction
	 */
	virtual bool DistinctSelect() const;

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
	virtual void ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins) const;

	/**
	 * Similar to ParseWhere, this method will go through the NameList and return a vector with the
	 * Relation/Attribute pair of the attributes. This will throw a runtime_error if an unknown
	 * relation or attribute is encountered. This method assumes that this objects Statistics
	 *  object has been populated with all relevant data.
	 * @param list	The NameList that will be parsed
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseNameList(struct NameList *list, std::vector<RelAttPair> &pair) const;

	/**
	 * Parses a FuncOperator and returns a vector of RelAttPair representing the attributes that
	 * are contained by func.
	 * @param func	The FuncOperator being parsed
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseFuncOperator(FuncOperator *func, std::vector<RelAttPair> &pair) const;

	/*
	 * Parses a TableList and returns a vector of RelAlias pair representing the aliases that
	 * correspond to each relation
	 * @param list	The TableList that contains the map of each alias
	 * @param pair	The vector of pairs that will be returned.
	 */
	virtual void ParseTableList(TableList *list, std::vector<RelAliasPair> &pairs) const;

	/**
	 * Parses table and puts the attributes into atts and the ordering into order. This will also
	 * throw an error if there is an attribute in order that does not appear in atts. Furthermore,
	 * this will expects the order in table to be put in reverse, so this will correctly order
	 * the attributes.
	 * @param table		The CreateTable being parsed
	 * @param atts		The atts and their type in CreateTable
	 * @param order		The order referenced in table
	 */
	virtual void ParseCreateTable(const CreateTable *table, std::vector<AttTypePair> &atts,
			std::vector<std::string> &order) const;

};

#endif /* INCLUDE_SQL_H_ */
