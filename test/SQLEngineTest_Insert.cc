#include "SQLEngineTest.h"
#include "RelationData.h"
#include "SQLEngine.h"
#include "SQL.h"
#include "DBFile.h"
#include <vector>
#include <string>

/**
 * SQLEngineTest::Insert should load file into the database associated with table
 */
TEST_F(SQLEngineTest, Insert1) {
	MockDBFile db;
	string file = "path/to/RelA.txt";
	string table = "RelA";
	Schema relASchema = relations["RelA"].schema;

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "data/DB/10M/", "catalog");

	InSequence seq;
	EXPECT_CALL(db, Open(StrEq("data/DB/10M/RelA.db"))).
			WillOnce(Return(1));

	EXPECT_CALL(db, Load(relASchema, StrEq("path/to/RelA.txt")));

	EXPECT_CALL(db, Close()).
			WillOnce(Return(1));

	Insert(test, sql, file, table, db);
}
