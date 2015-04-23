/*
 * SQLEngine.cc
 *
 *  Created on: Apr 22, 2015
 *      Author: cvasquez
 */

#include "SQLEngine.h"
#include "DBFile.h"
#include "SortedDBFile.h"
#include "Defs.h"

SQLEngine::SQLEngine() {
	// TODO Auto-generated constructor stub

}

SQLEngine::SQLEngine(string fileName) {
	// TODO Auto-generated constructor stub

}

SQLEngine::SQLEngine(const Statistics &_stats, const RelationData &_relations,
		const string &_dbPath, const std::string &_schemaLocation): stats(_stats),
				relations(_relations), dbPath(_dbPath), schemaLocation(_schemaLocation) {

}

SQLEngine::~SQLEngine() {
	// TODO Auto-generated destructor stub
}

const Statistics& SQLEngine::GetStatistics() const {
	return stats;
}

const RelationData& SQLEngine::GetRelations() const {
	return relations;
}

string SQLEngine::DBLocation(const std::string &table) const {
	string path = dbPath;
	path.append(table).append(".db");
	return path;
}

string SQLEngine::DBLocation(const char* table) const {
	return DBLocation(string(table));
}

SQL* SQLEngine::CreateSQL() const {
	return new SQL(stats);
}

void SQLEngine::CreateTable(SQL *sql, vector<AttTypePair> *atts, vector<string> *order,
		string tableName, DB_Type type) {
	DBFile db;
	CreateTable(sql, atts, order, tableName, type, db);
}

void SQLEngine::CreateTable(SQL *sql, vector<AttTypePair> *atts, vector<string> *order,
			string tableName, DB_Type type, DBFile &db) {
	// First, Populate Statistics
	stats.AddRel(tableName.c_str(), 0);

	for(int i = 0; i < atts->size(); i++) {
		stats.AddAtt(tableName.c_str(), atts->at(i).Attribute().c_str(), -1);
	}

	// Next, add the relation to relations
	Schema schema (*atts);
	string dbLocation = DBLocation(tableName);
	relations.Insert(tableName, dbLocation, schemaLocation, schema);

	// Lastly, create the new table
	if(type == Sorted) {
		int *runlen = new int(RUN_LENGTH);
		OrderMaker *sortorder = new OrderMaker(&schema, order);
		SortInfo *sort = new SortInfo(sortorder, runlen);
		db.Create(dbLocation.c_str(), sorted, sort);
	}
	else if(type == Heap) db.Create(dbLocation.c_str(), heap, NULL);

	// Close that thing
	db.Close();

	// Cleanup
	delete sql;
	delete atts;
	delete order;
}

void SQLEngine::Insert(SQL *sql, string file, string table) {
	DBFile db;
	Insert(sql, file, table, db);
}

void SQLEngine::Insert(SQL *sql, std::string file, std::string table, DBFile &db) {
	string tablepath = DBLocation(table);
	Schema schema = relations[table].schema;
	db.Open(tablepath.c_str());
	db.Load(schema, file.c_str());
	db.Close();
}

void SQLEngine::DropTable(SQL *sql, string table) {

}

void SQLEngine::DropTable(SQL *sql, string table, RawFile &rfile) {

}

void SQLEngine::Query(SQL *sql) const {

}

string SQLEngine::QueryPlan(SQL *sql) const {
	return "";
}
