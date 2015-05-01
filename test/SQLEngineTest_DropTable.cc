#include "SQLEngineTest.h"
#include "RelationData.h"
#include "SQLEngine.h"
#include "SQL.h"
#include "DBFile.h"
#include "RawFile.h"
#include <vector>
#include <string>

/**
 * SQLEngineTest::DropTable should remove the table from relations, stats, and it should delete the
 * local files.
 */
TEST_F(SQLEngineTest, DropTable1) {
	MockRawFile rfile;
	string table = "RelA";

	SQL *sql = new SQL(stat);
	SQLEngine test(stat, relations, "data/DB/10M/", "catalog");

	EXPECT_CALL(rfile, Remove(StrEq("data/DB/10M/RelA.db"))).
			WillOnce(Return(1));

	EXPECT_CALL(rfile, Remove(StrEq("data/DB/10M/RelA.db.header"))).
			WillOnce(Return(1));


	DropTable(test, sql, table, rfile);

	// Verify Relations
	RelationData testRels = test.GetRelations();
	ASSERT_THROW(testRels["RelA"], out_of_range);

	// Verify Statistics
	Statistics testStats = test.GetStatistics();
	EXPECT_EQ(-1, testStats.NumTuples("RelA"));
	EXPECT_EQ(-1, testStats.NumDistincts("RelA", "a"));
	EXPECT_EQ(-1, testStats.NumDistincts("RelA", "b"));
	EXPECT_EQ(-1, testStats.NumDistincts("RelA", "c"));
	EXPECT_EQ(-1, testStats.NumDistincts("RelA", "d"));
}
