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
#include "RawFile.h"

struct StatData {
	int numTuples;
	std::unordered_map<std::string, int> atts;
	std::set<std::string> set;
};

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
	virtual void AddRel(char *relName, int numTuples);

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
	virtual void AddAtt(char *relName, char *attName, int numDistincts);

	/**
	 * Returns the number of tuples in the provided relName.
	 * @param relName	The name of the relation whose statistics are being requested
	 * @return			The number of tuples in relName or NULL if no such relation exists
	 */
	virtual int NumTuples(char *relName);

	/**
	 * Returns the number of distinct tuples in the given attribute. This will return null if
	 * either the relation or attribute do not exist.
	 * @param relName	The name of the relation which holds the attribute
	 * @param attName	The name of the attribute being queried.
	 */
	virtual int NumDistincts(char *relName, char *attName);
   	void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Merges the sets containing rel1 and rel2. This will do nothing if both rel1 and rel2 are
	 * from the same set
	 * @param rel1	a relation in the first set
	 * @param rel2	a relation in the second set
	 */
	void MergeSets(std::string rel1, std::string rel2);

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
	bool VerifyJoin(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Checks to see if the attributes listed in parseTree belong to the relations in relNames.
	 * @return True if the above is true.
	 */
	bool VerifyJoinAttributes(struct AndList *parseTree, char **relNames, int numToJoin);

	/**
	 * Checks to see if the relations in relNames all completely belong to the same set(s). Ie,
	 * there is no relation in relNames that belongs to a set the contains another relation that is
	 * not in relNames.
	 * @param relNames	The relations that will be joined
	 * @param numToJoin	The number of relations in relNames
	 * @return True if the join is valid.
	 */
	bool VerifyJoinSets(char **relNames, int numToJoin);

	/**
	 * Returns the relation associated with att
	 * @param att	The attribute being looked up
	 * @return		The relation associated with att. Empty if no relation exists.
	 */
	std::string RelLookup(std::string att);

	/**
	 *  Returns the set with the associated relation
	 *  @param rel	The relation associated with the set returned
	 *  @return		The set associated with rel
	 */
	std::set<std::string> GetSet(std::string rel);
};

#endif /* INCLUDE_STATISTICS_H_ */
