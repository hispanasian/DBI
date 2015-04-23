#include "SQLTest.h"
#include "SQLTest.h"
#include "Defs.h"
#include "ParseTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * SQL::GetDropTable should correctly parse the input and return the expected data.
 */
TEST_F(SQLTest, GetDropTable1) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "DROP TABLE RelA; ";

	SQL test = SQL(stat);

	EXPECT_EQ(Drop_Table, test.Parse(str));

	EXPECT_EQ(0, test.GetDropTable().compare("RelA"));
}


/**
 * SQL::GetDropTable should throw an error if the table does not exist
 */
TEST_F(SQLTest, GetDropTable2) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "DROP TABLE RelB; ";

	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Drop_Table, test.Parse(str));
	ASSERT_THROW(test.GetDropTable(), invalid_argument);
}
