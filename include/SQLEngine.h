/*
 * SQLEngine.h
 *
 *  Created on: Apr 22, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SQLENGINE_H_
#define INCLUDE_SQLENGINE_H_

#include "SQL.h"
#include "Statistics.h"
#include "RelationData.h"
#include "DBFile.h"

/**
 * SQLEngine maintains and keeps track of all the data structures that are used to execute a SQL
 * command. This includes:
 * Relations
 * Schemas
 * Statistics
 *
 * SQLEngine also contains the instructions for handling the different SQL commands
 */
class SQLEngine {
	friend class SQLEngineTest;
protected:
	Statistics stats;
	RelationData relations;
	std::string dbPath;
	std::string schemaLocation;

	virtual void CreateTable(SQL *sql, std::vector<AttTypePair> *atts, std::vector<std::string> *order,
				std::string tableName, DB_Type type, DBFile &db);
public:

	/**
	 * Returns the statistics object associated with this SQL Engine
	 * @return The statistics object associated with this SQL Engine
	 */
	const Statistics& GetStatistics() const;

	/**
	 * Returns the RelationData object associated with this SQL Engine
	 * @param Returns the RelationData object associated with this SQL Engine
	 */
	const RelationData& GetRelations() const;

	/**
	 * Creates a new SQL Engine
	 */
	SQLEngine();

	/**
	 * Creates a new SQL Engine with the data loaded from startFile
	 */
	SQLEngine(std::string startFile);

	SQLEngine(const Statistics &_stats, const RelationData &_relations, const std::string &_dbPath, const std::string &_schemaLocation);
	virtual ~SQLEngine();

	/**
	 * Returns a new SQL object that contains the required SQL data
	 * @return A new SQL object that contains the required SQL data
	 */
	virtual SQL* CreateSQL() const ;

	/**
	 * Adds the table with tableName, atts, and order (if applicable) to this engine. The data will
	 * be cleaned up by this method. This method assumes that the table does not exist and that
	 * this has already been checked
	 * @param sql			The SQL object that parsed the data for this call
	 * @param atts			The Attributes that will be put into this relation
	 * @param order			The ordering of the attributes for this relation
	 * @param tableName		The name of this new Relation
	 * @param type			The type of the new table
	 */
	virtual void CreateTable(SQL *sql, std::vector<AttTypePair> *atts, std::vector<std::string> *order,
			std::string tableName, DB_Type type);

	/**
	 * Inserts the file located at file into the table. The objects will be cleaned up by this
	 * method. This method assumes that the table does exist.
	 * @param sql			The SQL object that parsed the data for this call
	 * @param file			The path to the file from which the data will be loaded
	 * @param table			The table in which the data at file will be loaded
	 */
	virtual void Insert(SQL *sql, std::string file, std::string table);

	/**
	 * Removes table from associated structures. The objects will be cleaned up by this method.
	 * This method assumes that the table exists.
	 * @param sql			The SQL object that parsed the data for this call
	 * @param table			The table that will be dropped
	 */
	virtual void DropTable(SQL *sql, std::string table);

	/**
	 * Runs the provided SQL query
	 * @param sql			The SQL object that parsed the data for this call
	 */
	virtual void Query(SQL *sql) const;

	/**
	 * Returns a string that contains the QueryPlan for the query held by sql
	 * @param sql			The SQL object that parsed the data for this call
	 * @return 				A string that contains the QueryPlan for the query held by sql
	 */
	virtual std::string QueryPlan(SQL *sql) const;
};

#endif /* INCLUDE_SQLENGINE_H_ */
