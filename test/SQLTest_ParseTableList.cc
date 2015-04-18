#include "SQLTest.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//
//string NameListToString(NameList *list) {
//	string temp = "";
//
//	while(list != NULL) {
//		temp.append(list->name);
//		list = list->next;
//	}
//	return temp;
//}

/**
 * ParseTableList should return all the relations referenced in the FROM clause as well as their
 * alias
 */
TEST_F(SQLTest, ParseTableList1) {
	char *relName[] = {"A", "B", "C"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);
	stat.AddRel(relName[2],6001215);

	string sql = "SELECT A.a1, b1, b2 ";
	sql.append(" FROM A AS a, B AS b, C AS c ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAliasPair> pair;
	SQL test = SQL(stat);
	test.ParseTableList(tables, pair);

	ASSERT_EQ(3, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Alias().compare("a") == 0);
	EXPECT_TRUE(pair[1].Relation().compare("B") == 0);
	EXPECT_TRUE(pair[1].Alias().compare("b") == 0);
	EXPECT_TRUE(pair[2].Relation().compare("C") == 0);
	EXPECT_TRUE(pair[2].Alias().compare("c") == 0);
}

/**
 * ParseTableList should not fail when there is only one element
 */
TEST_F(SQLTest, ParseTableList2) {
	char *relName[] = {"A", "B", "C"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);
	stat.AddRel(relName[2],6001215);

	string sql = "SELECT A.a1 ";
	sql.append(" FROM A AS a ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAliasPair> pair;
	SQL test = SQL(stat);
	test.ParseTableList(tables, pair);

	ASSERT_EQ(1, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Alias().compare("a") == 0);
}

/**
 * ParseTableList should throw a runtime_error if the Relation
 */
TEST_F(SQLTest, ParseTableList3) {
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

	vector<RelAliasPair> pair;
	SQL test = SQL(stat);
	ASSERT_THROW(test.ParseTableList(tables, pair), runtime_error);
}
