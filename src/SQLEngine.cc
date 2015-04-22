/*
 * SQLEngine.cc
 *
 *  Created on: Apr 22, 2015
 *      Author: cvasquez
 */

#include "SQLEngine.h"

SQLEngine::SQLEngine() {
	// TODO Auto-generated constructor stub

}

SQLEngine::SQLEngine(string fileName) {
	// TODO Auto-generated constructor stub

}

SQLEngine::SQLEngine(const Statistics &_stats, const RelationData &_relations,
		const string &_dbPath): stats(_stats), relations(_relations), dbPath(_dbPath) {

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

SQL* SQLEngine::CreateSQL() const {
	return new SQL(stats);
}

void SQLEngine::CreateTable(SQL *sql, vector<AttTypePair> *atts, vector<string> *order,
		string *tableName, DB_Type type) {
	DBFile db;
	CreateTable(sql, atts, order, tableName, type, db);
}

void SQLEngine::CreateTable(SQL *sql, vector<AttTypePair> *atts, vector<string> *order,
			string *tableName, DB_Type type, DBFile &db) {

}

void SQLEngine::Insert(SQL *sql, string *file, string *table) {

}


void SQLEngine::DropTable(SQL *sql, string *table) {

}

void SQLEngine::Query(SQL *sql) const {

}

string SQLEngine::QueryPlan(SQL *sql) const {
	return "";
}
