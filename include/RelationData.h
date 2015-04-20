/*
 * RelationData.h
 *
 *  Created on: Apr 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_RELATIONDATA_H_
#define INCLUDE_RELATIONDATA_H_

#include "Schema.h"
#include <string>
#include <unordered_map>

/**
 * Relation data will contain information regarding a given relation. The data includs:
 * - The file of the database associated with the relation
 * - The schema of the relation
 * - The file of the schema ascociated with the relation
 */
class RelationMetaData {
public:
	const std::string dbLocation;
	const std::string schemaLocation;
	const Schema schema;

	RelationMetaData();
	RelationMetaData(const std::string &_dbLocation, const std::string &_schemaLocation,
			const Schema &_schema);
	RelationMetaData(const RelationMetaData &copyMe);
	~RelationMetaData();
};

class RelationData {
protected:
	unordered_map<std::string, RelationMetaData> data;
public:
	RelationData();
	RelationData(const RelationData &copyMe);
	virtual ~RelationData();

	/**
	 * Inserts the db location, schema location, and schema associated with rel into this object
	 */
	void Insert(const std::string &rel, const std::string &dbLocation,
			const std::string &schemaLocation, const Schema &schema);

	void Insert(const std::string &rel, const RelationMetaData &copy);

	const RelationMetaData& operator [](std::string rel) const { return data.at(rel); }

	std::string ToString() const;
};

#endif /* INCLUDE_RELATIONDATA_H_ */
