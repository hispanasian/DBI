#include "SQLEngineTest.h"
#include "SQL.h"
#include "Defs.h"
#include "ParseTree.h"
#include <vector>

SQLEngineTest::SQLEngineTest() {
	stat.AddRel("RelA", 0);
	stat.AddAtt("RelA", "a", 0);
	stat.AddAtt("RelA", "b", 0);
	stat.AddAtt("RelA", "c", 0);
	stat.AddAtt("RelA", "d", 0);

	vector<AttTypePair> temp;
	temp.push_back(AttTypePair("a", INT));
	temp.push_back(AttTypePair("b", INT));
	temp.push_back(AttTypePair("c", DOUBLE));
	temp.push_back(AttTypePair("d", STRING));

	relations.Insert("RelA", "data/DB/10M/", "data/catalog", Schema(temp));
}

SQLEngineTest::~SQLEngineTest() {

}

void SQLEngineTest::CreateTable(SQLEngine &eng, SQL *sql, vector<AttTypePair> *atts, vector<string> *order,
		string tableName, DB_Type type, DBFile &db) {
	eng.CreateTable(sql, atts, order, tableName, type, db);
}
