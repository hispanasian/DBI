#include "SQLEngineTest.h"
#include "RelationData.h"
#include "SQLEngine.h"
#include "SQL.h"
#include "DBFile.h"
#include <vector>
#include <string>

/**
 * SQLEngineTest::CreateTable should create a Heap
 */
TEST_F(SQLEngineTest, CreateTable1) {
	StrictMock<MockDBFile> db;

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "test.cat");

	vector<AttTypePair> *atts = new vector<AttTypePair>;
	vector<string> *order = new vector<string>;
	DB_Type type = Heap;

	atts->push_back(AttTypePair("att1", Int));
	atts->push_back(AttTypePair("att2", Double));
	atts->push_back(AttTypePair("att3", String));

	Schema relb (*atts);

	EXPECT_CALL(db, Create("data/DB/10M/RelB", heap, NULL)).
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
	StrictMock<MockDBFile> db;

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "test.cat");

	vector<AttTypePair> *atts = new vector<AttTypePair>;
	vector<string> *order = new vector<string>;
	DB_Type type = Sorted;

	atts->push_back(AttTypePair("att1", Int));
	atts->push_back(AttTypePair("att2", Double));
	atts->push_back(AttTypePair("att3", String));
	order->push_back("att2");

	Schema relb (*atts);

	EXPECT_CALL(db, Create("data/DB/10M/RelB", sorted, NotNull())).
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
