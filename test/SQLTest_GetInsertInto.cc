#include "SQLTest.h"
#include "SQLTest.h"
#include "Defs.h"
#include "ParseTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * SQL::GetInsertInto should correctly parse the input and return the expected data.
 */
TEST_F(SQLTest, GetInsertInto1) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something.txt INTO RelA; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	test.GetInsertInto(file, table);

	EXPECT_EQ(0, file.compare("something.txt"));
	EXPECT_EQ(0, table.compare("RelA"));
}


/**
 * SQL::GetInsertInto should throw an error if the table does not exist
 */
TEST_F(SQLTest, GetInsertInto2) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something.txt INTO RelB; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	ASSERT_THROW(test.GetInsertInto(file, table);, invalid_argument);
}


/**
 * SQL::GetInsertInto should correctly parse the input and return the expected data even when
 * the file name does not have a '.'
 */
TEST_F(SQLTest, GetInsertInto3) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something INTO RelA; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	test.GetInsertInto(file, table);

	EXPECT_EQ(0, file.compare("something"));
	EXPECT_EQ(0, table.compare("RelA"));
}

/**
 * SQL::GetInsertInto should correctly parse the input and return the expected data even when
 * the file name has multiple '.'s
 */
TEST_F(SQLTest, GetInsertInto4) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something.raw.db INTO RelA; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	test.GetInsertInto(file, table);

	EXPECT_EQ(0, file.compare("something.raw.db"));
	EXPECT_EQ(0, table.compare("RelA"));
}
