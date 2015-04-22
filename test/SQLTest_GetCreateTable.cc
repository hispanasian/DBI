#include "SQLTest.h"
#include "SQLTest.h"
#include "Defs.h"
#include "ParseTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * SQL::GetCreateTable should correctly parse the input and return the expected data.
 */
TEST_F(SQLTest, GetCreateTable1) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE mytable (att1 INTEGER, att2 DOUBLE, att3 STRING) AS HEAP; ";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	EXPECT_EQ(Heap, test.GetCreateTable(atts, order, tableName));

	ASSERT_EQ(3, atts.size());
	ASSERT_EQ(0, order.size());
	EXPECT_EQ(0, tableName.compare("mytable"));

	EXPECT_EQ(0, atts[0].Attribute().compare("att1"));
	EXPECT_EQ(0, atts[1].Attribute().compare("att2"));
	EXPECT_EQ(0, atts[2].Attribute().compare("att3"));

	EXPECT_EQ(Int, atts[0].GetType());
	EXPECT_EQ(Double, atts[1].GetType());
	EXPECT_EQ(String, atts[2].GetType());
}

/**
 * SQL::GetCreateTable should return items in order if the table is Sorted
 */
TEST_F(SQLTest, GetCreateTable2) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE MYTABLE (att1 INTEGER, att2 DOUBLE, att3 STRING) AS SORTED ON att1, att2;";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	EXPECT_EQ(Sorted, test.GetCreateTable(atts, order, tableName));

	ASSERT_EQ(3, atts.size());
	ASSERT_EQ(2, order.size());
	EXPECT_EQ(0, tableName.compare("MYTABLE"));

	EXPECT_EQ(0, atts[0].Attribute().compare("att1"));
	EXPECT_EQ(0, atts[1].Attribute().compare("att2"));
	EXPECT_EQ(0, atts[2].Attribute().compare("att3"));

	EXPECT_EQ(Int, atts[0].GetType());
	EXPECT_EQ(Double, atts[1].GetType());
	EXPECT_EQ(String, atts[2].GetType());

	EXPECT_EQ(0, order[0].compare("att1"));
	EXPECT_EQ(0, order[1].compare("att2"));
}

/**
 * SQL::GetCreateTable should throw an error if the table already exists
 */
TEST_F(SQLTest, GetCreateTable3) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE RelA (att1 INTEGER, att2 DOUBLE, att3 STRING) AS SORTED ON att1, att2;";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	ASSERT_THROW(test.GetCreateTable(atts, order, tableName), invalid_argument);
}

/**
 * SQL::GetCreateTable should throw an exception if the sorted table says to sort on attributes
 * which were not mentioned in the attribute list
 */
TEST_F(SQLTest, GetCreateTable4) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE MYTABLE (att1 INTEGER, att2 DOUBLE, att3 STRING) AS SORTED ON att1, att5;";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	ASSERT_THROW(test.GetCreateTable(atts, order, tableName), invalid_argument);
}

/**
 * SQL::GetCreateTable should throw an exception if there are no attributes put in SORTED
 */
TEST_F(SQLTest, GetCreateTable5) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE MYTABLE (att1 INTEGER, att2 DOUBLE, att3 STRING) AS SORTED ON ;";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	ASSERT_THROW(test.GetCreateTable(atts, order, tableName), invalid_argument);
}
