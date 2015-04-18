#include "SQLTest.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

string FuncOperatorToString(FuncOperator *func) {
	string temp;
	if(func != NULL) {
		// First, parse left child
		temp.append(" ");
		temp.append(FuncOperatorToString(func->leftOperator));
		temp.append(" ");

		// Next, check root.
		struct FuncOperand *op = func->leftOperand;
		temp.append(" ");
		temp.append(op->value);
		temp.append(" ");

		// Finally, check right child
		temp.append(" ");
		temp.append(FuncOperatorToString(func->right));
		temp.append(" ");
	}
	return temp;
}

/**
 * ParseFuncOperator should return all the relation/attrtibute pairs contained in a function.
 */
TEST_F(SQLTest, ParseFuncOperator1) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT SUM( (A.a1 + 5)/(A.a2 - B.b3) )";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
//	cout << finalFunction << endl;
//	cout << FuncOperatorToString(finalFunction);
	test.ParseFuncOperator(finalFunction, pair);

	ASSERT_EQ(3, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Attribute().compare("a1") == 0);
	EXPECT_TRUE(pair[1].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[1].Attribute().compare("a2") == 0);
	EXPECT_TRUE(pair[2].Relation().compare("B") == 0);
	EXPECT_TRUE(pair[2].Attribute().compare("b3") == 0);
}

/**
 * ParseFuncOperator should throw a runtime_error if a relation/attribute pair does not exist
 */
TEST_F(SQLTest, ParseFuncOperator2) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	string sql = "SELECT SUM( (A.a1 + 5)/(A.a2 - B.b6))";
	sql.append(" FROM rel AS A ");
	sql.append(" WHERE (A.a1 = 5)");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAttPair> pair;
	SQL test = SQL(stat);
	ASSERT_THROW(test.ParseFuncOperator(finalFunction, pair), runtime_error);
}
