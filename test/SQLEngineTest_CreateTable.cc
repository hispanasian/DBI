#include "SQLEngineTest.h"
#include "RelationData.h"
#include "SQL.h"
#include <vector>
#include <string>

/**
 * SQLEngineTest::CreateTable should create a
 */
TEST_F(SQLEngineTest, CreateTable1) {
	SQL sql(stat);
	SQLEngine test(stat, relations, "test.cat");

	vector<AttTypePair> atts;
	vector<string> order;
	DB_Type type = Heap;
}
