#include "SQLEngineTest.h"
#include "RelationData.h"
#include "SQLEngine.h"
#include "SQL.h"
#include "DBFile.h"
#include <vector>
#include <string>

using ::testing::StrEq;

/**
 * SQLEngineTest::CreateTable should create a Heap
 */
TEST_F(SQLEngineTest, CreateTable1) {
	MockDBFile db;

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "data/DB/10M/", "catalog");

	vector<AttTypePair> *atts = new vector<AttTypePair>;
	vector<string> *order = new vector<string>;
	DB_Type type = Heap;

	atts->push_back(AttTypePair("att1", Int));
	atts->push_back(AttTypePair("att2", Double));
	atts->push_back(AttTypePair("att3", String));

	Schema relb (*atts);

	EXPECT_CALL(db, Create(StrEq("data/DB/10M/RelB.db"), heap, NULL)).
//	EXPECT_CALL(db, Create(_, _, _)).
			WillOnce(Return(1));

	EXPECT_CALL(db, Close()).
			WillOnce(Return(1));

	CreateTable(test, sql, atts, order, "RelB", type, db);

	Statistics testStats = test.GetStatistics();
	RelationData testRels = test.GetRelations();

	// Verify Statistics
	EXPECT_NE(-1, testStats.NumTuples("RelB"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att1"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att2"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att3"));

	// Verify Relations
	EXPECT_TRUE(relb == testRels["RelB"].schema);
	EXPECT_TRUE(testRels["RelB"].dbLocation.compare("data/DB/10M/RelB.db") == 0);
}

/**
 * SQLEngineTest::CreateTable should create a Sorted DB
 */
TEST_F(SQLEngineTest, CreateTable2) {
	MockDBFile db;

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "data/DB/10M/", "catalog");

	vector<AttTypePair> *atts = new vector<AttTypePair>;
	vector<string> *order = new vector<string>;
	DB_Type type = Sorted;

	atts->push_back(AttTypePair("att1", Int));
	atts->push_back(AttTypePair("att2", Double));
	atts->push_back(AttTypePair("att3", String));
	order->push_back("att2");

	Schema relb (*atts);

	EXPECT_CALL(db, Create(StrEq("data/DB/10M/RelB.db"), sorted, NotNull())).
			WillOnce(Return(1));

	EXPECT_CALL(db, Close()).
			WillOnce(Return(1));

	CreateTable(test, sql, atts, order, "RelB", type, db);

	Statistics testStats = test.GetStatistics();
	RelationData testRels = test.GetRelations();

	// Verify Statistics
	EXPECT_NE(-1, testStats.NumTuples("RelB"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att1"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att2"));
	EXPECT_NE(-1, testStats.NumDistincts("RelB", "att3"));

	// Verify Relations
	EXPECT_TRUE(relb == testRels["RelB"].schema);
	EXPECT_TRUE(testRels["RelB"].dbLocation.compare("data/DB/10M/RelB.db") == 0);
}
