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
	 * Returns the number of tuples in the provided relName.
	 * @param relName	The name of the relation whose statistics are being requested
	 * @return			The number of tuples in relName or NULL if no such relation exists
	 */
	virtual int NumTuples(char *relName);
};

#endif /* INCLUDE_STATISTICS_H_ */
