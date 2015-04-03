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
	virtual void Read(char *fromWhere, RawFile &file);
	virtual void Write(char *fromWhere, RawFile &file);
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
	 * Returns the number of tuples in the provided relName.
	 * @param relName	The name of the relation whose statistics are being requested
	 * @return			The number of tuples in relName or NULL if no such relation exists
	 */
	virtual int NumTuples(char *relName);
};

#endif /* INCLUDE_STATISTICS_H_ */
