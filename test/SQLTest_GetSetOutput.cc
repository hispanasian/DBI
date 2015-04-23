#include "SQLTest.h"
#include "SQLTest.h"
#include "Defs.h"
#include "ParseTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * SQL::GetSetOutput should return with STDOUT
 */
TEST_F(SQLTest, GetSetOutput1) {
	char *relName[] = {"RelA"};
	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT STDOUT; ";

	string file;
	SQL test = SQL(stat);

	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(Stdout, test.GetSetOutput(file));
}


/**
 * SQL::GetSetOutput should return with NONE
 */
TEST_F(SQLTest, GetSetOutput2) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT NONE; ";

	string file;
	SQL test = SQL(stat);

	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(None, test.GetSetOutput(file));
}


/**
 * SQL::GetSetOutput should return with File_Path and provide the correct file path
 */
TEST_F(SQLTest, GetSetOutput3) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT path.txt; ";

	string file;
	SQL test = SQL(stat);

	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(File_Path, test.GetSetOutput(file));
	EXPECT_EQ(0, file.compare("path.txt"));
}

/**
 * SQL::GetSetOutput should return with File_Path and provide the correct file path and should
 * handle the case of multiple 's's
 */
TEST_F(SQLTest, GetSetOutput4) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT file.raw.db; ";

	string file;
	SQL test = SQL(stat);
;
	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(File_Path, test.GetSetOutput(file));
	EXPECT_EQ(0, file.compare("file.raw.db"));
}

/**
 * SQL::GetSetOutput should return with File_Path and provide the correct file path and should
 * handle the case of no '.'
 */
TEST_F(SQLTest, GetSetOutput5) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT path/to/file; ";

	string file;
	SQL test = SQL(stat);

	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(File_Path, test.GetSetOutput(file));
	EXPECT_EQ(0, file.compare("path/to/file"));
}
