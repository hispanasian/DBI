/*
 * Statistics.h
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#include <unordered_map>
#include <string>
#include "RawFile.h"

struct StatPair {
	int numTuples;
	std::unordered_map<std::string, int> atts;
};

class Statistics {
friend class StatisticsTest;

protected:
	std::unordered_map<std::string, StatPair> &relations;
	std::unordered_map<std::string, StatPair> myRelations;

	Statistics(std::unordered_map<std::string, StatPair> &_relations);
public:
	Statistics();
	virtual ~Statistics();

	/**
	 * Adds (or replaces if it exists) a base relation to this structure.
	 * @param relName	The name of the base relation
	 * @param numTuples	The number of tuples in the relation
	 */
	virtual void AddRel(char *relName, int numTuples);

	/**
	 * Copies the relation (including all of its attributes and statistics) from oldName to a
	 * relation with the name newName. If old_name does not exist, a out_of_range exception.
	 * @param oldName	The name of the relation to be copied
	 * @param newName	The name of the relation that will contain the copy of oldName
	 */
	virtual void CopyRel(char *oldName, char *newName);

    /*
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
};

#endif /* INCLUDE_STATISTICS_H_ */
