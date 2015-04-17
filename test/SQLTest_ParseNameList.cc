#include "SQLTest.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

string NameListToString(NameList *list) {
	string temp = "";

	while(list != NULL) {
		temp.append(list->name);
		list = list->next;
	}
	return temp;
}

/**
 * ParseNameList should return the NameList in reverse order (because it is put in reverse order
 * when parsing)
 */
TEST_F(SQLTest, ParseNameList1) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT A.a1, b1, b2 ";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
	test.ParseNameList(attsToSelect, pair);

	ASSERT_EQ(3, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Attribute().compare("a1") == 0);
	EXPECT_TRUE(pair[1].Relation().compare("B") == 0);
	EXPECT_TRUE(pair[1].Attribute().compare("b1") == 0);
	EXPECT_TRUE(pair[2].Relation().compare("B") == 0);
	EXPECT_TRUE(pair[2].Attribute().compare("b2") == 0);
}

/**
 * ParseNameList should not fail when there is only one element
 */
TEST_F(SQLTest, ParseNameList2) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT A.a1 ";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
	test.ParseNameList(attsToSelect, pair);

	ASSERT_EQ(1, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Attribute().compare("a1") == 0);
}

/**
 * ParseNameList should throw a runtime_error if the attribute does not exist
 */
TEST_F(SQLTest, ParseNameList3) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT a5 ";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
	ASSERT_THROW(test.ParseNameList(attsToSelect, pair), runtime_error);
}

/**
 * ParseNameList should throw a runtime_error if the relation does not exist
 */
TEST_F(SQLTest, ParseNameList4) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT C.a5 ";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
	ASSERT_THROW(test.ParseNameList(attsToSelect, pair), runtime_error);
}

