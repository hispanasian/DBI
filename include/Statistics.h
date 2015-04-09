/*
 * Statistics.h
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "RawFile.h"
#include "ParseTree.h"
#include "Expression.h"

struct StatData {
	double numTuples;
	std::unordered_map<std::string, int> atts;
	std::set<std::string> set;
};

class Expression;

class Statistics {
friend class StatisticsTest;

protected:
	std::unordered_map<std::string, StatData> &relations;
	std::unordered_map<std::string, StatData> myRelations;
	std::unordered_map<std::string, std::string> &lookup; // att -> rel mapping
	std::unordered_map<std::string, std::string> myLookup;

	Statistics(std::unordered_map<std::string, StatData> &_relations, std::unordered_map<std::string, std::string> &_lookup);
	virtual void Read(char *fromWhere, RawFile &file);
	virtual void Write(char *fromWhere, RawFile &file);

	/**
	 * Adds (or replaces if it exists) a base relation to this structure. This does not affect the
	 * relations contained by the set of this relation.
	 * @param relName	The name of the base relation
	 * @param numTuples	The number of tuples in the relation
	 */
	virtual void UpdateRel(const char *relName, int numTuples);
public:
	Statistics();
	Statistics(const Statistics &copyMe);
	virtual ~Statistics();

	/**
	 * Adds (or replaces if it exists) a base relation to this structure and any other relations
	 * that exist in the set contained by this relation.
	 * @param relName	The name of the base relation
	 * @param numTuples	The number of tuples in the relation
	 */
	virtual void AddRel(char *relName, double numTuples);

	/**
	 * De-serializes the contents from the file located at fromWhere and puts it into this object.
	 * The format of the file should be as follows:
	 *
	 * Relations
	 * rel1=numTuples
	 * reln=numTuples
	 * Attributes
	 * rel1.att1=numDistinct
	 * reln.attj=numDistinct
	 *
	 * This method will throw a out_of_range exception if there is a relation.attribute pair for
	 * which the relation does not exist (it was not declared in the Relations section)
	 * @param fromWhere	The file containing a serialized contents of a Statistics object
	 */
	virtual void Read(char *fromWhere);

	/**
	 * Serializes the contents of Statistics to the file located at fromWhere
	 * @param fromWhere	The file that will hold the serialized contents of this object
	 */
	virtual void Write(char *fromWhere);

    /**
	 * Copies the relation (including all of its attributes and statistics) from oldName to a
	 * relation with the name newName. If old_name does not exist, a out_of_range exception.
	 * Note that CopyRel will NOT copy the set from oldName. newName will only have itself in its
	 * set after CopyRel exectutes.
	 * @param oldName	The name of the relation to be copied
	 * @param newName	The name of the relation that will contain the copy of oldName
	 */
	virtual void CopyRel(char *oldName, char *newName);

    /**
	 * Adds (or replaces if it exists) an attribute to this structure. A numDistincts of -1 will
	 * signify that the number of distincts is assumed to be equal to the number of tuples in the
	 * associated relation. Furthermore, this will throw a out_of_range exception if a relation
	 * with relName does not exist.
	 * @param relName	The base relation of the attribute
	 * @param attName	The attribute's name
	 * @numDistincts	The number of distinct attributes
	 */
	virtual void AddAtt(char *relName, char *attName, double numDistincts);

	/**
	 * Returns the number of tuples in the provided relName.
	 * @param relName	The name of the relation whose statistics are being requested
	 * @return			The number of tuples in relName or -1 if no such relation exists
	 */
	virtual double NumTuples(const char *relName);

	/**
	 * Returns the number of distinct tuples in the given attribute. This will return null if
	 * either the relation or attribute do not exist.
	 * @param relName	The name of the relation which holds the attribute
	 * @param attName	The name of the attribute being queried or -1 if neither the relation nor
	 * 					attribute exist
	 */
	virtual double NumDistincts(const char *relName, const char *attName);
	//virtual int NumDistincts(const char *relName, const char *attName);
   	virtual void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	virtual double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Merges the sets containing rel1 and rel2. This will do nothing if both rel1 and rel2 are
	 * from the same set
	 * @param rel1	a relation in the first set
	 * @param rel2	a relation in the second set
	 */
	virtual void MergeSets(std::string rel1, std::string rel2);

	/**
	 * Checks to see if the relations in relNames can be joined. The relations can be joined if:
	 * 1. The attributes listed in parseTree belong to the relations in relNames
	 * 2. The relations in relNames all completely belong to the same set(s). Ie, there is no
	 * relation in relNames that belongs to a set the contains another relation that is not in
	 * relNames.
	 * @param parseTree	The cnf of the join
	 * @param relNames	The relations that will be joined
	 * @param numToJoin	The number of relations in relNames
	 * @return True if the join is valid.
	 */
	virtual bool VerifyJoin(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Checks to see if the attributes listed in parseTree belong to the relations in relNames.
	 * @return True if the above is true.
	 */
	virtual bool VerifyJoinAttributes(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Checks to see if the relations in relNames all completely belong to the same set(s). Ie,
	 * there is no relation in relNames that belongs to a set the contains another relation that is
	 * not in relNames.
	 * @param relNames	The relations that will be joined
	 * @param numToJoin	The number of relations in relNames
	 * @return True if the join is valid.
	 */
	virtual bool VerifyJoinSets(char **relNames, int numToJoin);

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

	/**
	 * Returns the relation associated with att
	 * @param att	The attribute being looked up
	 * @return		The relation associated with att. Empty if no relation exists.
	 */
	virtual std::string RelLookup(std::string att);

	/**
	 *  Returns the set with the associated relation
	 *  @param rel	The relation associated with the set returned
	 *  @return		The set associated with rel
	 */
   	//void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	//double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);
	virtual std::set<std::string> GetSet(std::string rel);

	/**
	 *	Parses the given comparison op, creates an appropriate Expression and appends it 
	 *	to the expression list. Adds all relations referenced in the created Expression
	 *	to the set of relations.
	 *	If the expression is a comparison between 2 literals, this method throws and exception.
	*/
	 virtual void MakeExpression(ComparisonOp op, std::vector<Expression*>& expressions,
	 	std::set<std::string>& relations);
};

#endif /* INCLUDE_STATISTICS_H_ */
