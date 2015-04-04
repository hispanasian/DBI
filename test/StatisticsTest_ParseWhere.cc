#include "StatisticsTest.h"

/**
 * ParseWhere should create two AndLists for select and two AndLists for Join where both AndLists
 * for join are the same and the two AndLists for the Selects match the CNF provided. Ordering of
 * the CNF should not matter, it should only be expected that any OrList that is not a binary
 * expression only relate to one relation
 */
TEST_F(StatisticsTest, ParseWhere1) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	char *cnf = "(b1 = 5) AND (a1=b3) AND (a1 > 5 OR a2 = 9 OR a3 < 10)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	stat.ParseWhere(final, selects, joins);

	EXPECT_EQ(2, selects.size());
	EXPECT_EQ(2, joins.size());
	EXPECT_EQ(1, joins.at("A").size());
	EXPECT_EQ(1, joins.at("B").size());

	ASSERT_TRUE(selects.at("A") != NULL);
	ASSERT_TRUE(selects.at("B") != NULL);
	ASSERT_TRUE(joins.at("A").at("B") != NULL);
	ASSERT_TRUE(joins.at("B").at("A") != NULL);
}

/**
 * ParseWhere should be able to handle the case where no Joins exist (this implies that there is
 * only on Select on one relation)
 */
TEST_F(StatisticsTest, ParseWhere2) {
	char *relName[] = {"A"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);

	char *cnf = "(a1 > 5 OR a2 = 9 OR a3 < 10)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	stat.ParseWhere(final, selects, joins);

	EXPECT_EQ(1, selects.size());
	EXPECT_EQ(0, joins.size());

	ASSERT_TRUE(selects.at("A") != NULL);
}

/**
 * ParseWhere should be throw a runtime_error if not all the relations are joined (when there is
 * more than one relation)
 */
TEST_F(StatisticsTest, ParseWhere3) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	char *cnf = "(b1 = 5) AND (a1 > 5 OR a2 = 9 OR a3 < 10)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	ASSERT_THROW(stat.ParseWhere(final, selects, joins), runtime_error);
}

/**
 * ParseWhere should handle the case where no Select statements exist
 */
TEST_F(StatisticsTest, ParseWhere4) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	char *cnf = "(a1=b3)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	stat.ParseWhere(final, selects, joins);

	EXPECT_EQ(0, selects.size());
	EXPECT_EQ(2, joins.size());
	EXPECT_EQ(1, joins.at("A").size());
	EXPECT_EQ(1, joins.at("B").size());

	ASSERT_THROW(selects.at("A"), out_of_range);
	ASSERT_THROW(selects.at("B"), out_of_range);
	ASSERT_TRUE(joins.at("A").at("B") != NULL);
	ASSERT_TRUE(joins.at("B").at("A") != NULL);
}

/**
 * ParseWhere should handle more complicated cnfs
 */
TEST_F(StatisticsTest, ParseWhere5) {
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
	stat.AddAtt(relName[2], "c1",3);

	char *cnf = "(b1 = 5) AND (a1=b3) AND (a1 > 5 OR a2 = 9 OR a3 < 10) AND (c1=b1)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	stat.ParseWhere(final, selects, joins);

	EXPECT_EQ(2, selects.size());
	EXPECT_EQ(3, joins.size());

	ASSERT_TRUE(selects.at("A") != NULL);
	ASSERT_TRUE(selects.at("B") != NULL);
	ASSERT_TRUE(joins.at("A").at("B") != NULL);
	ASSERT_TRUE(joins.at("B").at("A") != NULL);
	ASSERT_TRUE(joins.at("B").at("C") != NULL);
	ASSERT_TRUE(joins.at("C").at("B") != NULL);
}

/**
 * ParseWhere should check for an invalid (insufficient) number of joins
 */
TEST_F(StatisticsTest, ParseWhere6) {
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
	stat.AddAtt(relName[2], "c1",3);

	char *cnf = "(b1 = 5) AND (a1=b3) AND (a1 > 5 OR a2 = 9 OR a3 < 10)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	ASSERT_THROW(stat.ParseWhere(final, selects, joins), runtime_error);
}

/**
 * ParseWhere should check for an invalid (excessive) number of joins (implying multiple joins on
 * the same relation)
 */
TEST_F(StatisticsTest, ParseWhere7) {
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
	stat.AddAtt(relName[2], "c1",3);

	char *cnf = "(b1 = 5) AND (a1=b3) AND (a1 > 5 OR a2 = 9 OR a3 < 10) AND (c1=a1) AND (a2=b2)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	ASSERT_THROW(stat.ParseWhere(final, selects, joins), runtime_error);
}

/**
 * ParseWhere should be able to handle multiple OrLists (selects) that affect the same relation
 */
TEST_F(StatisticsTest, ParseWhere8) {
	char *relName[] = {"A", "B", "C"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);

	char *cnf = "(a1=15 OR a2<5) AND (a3=6)";

	yy_scan_string(cnf);
	yyparse();

	unordered_map<string, AndList*> selects;
	unordered_map<string, unordered_map<string, AndList*> > joins;

	stat.ParseWhere(final, selects, joins);

	EXPECT_EQ(1, selects.size());
	EXPECT_EQ(0, joins.size());

	ASSERT_TRUE(selects.at("A") != NULL);
	ASSERT_TRUE(selects["A"]->left != NULL);
	ASSERT_TRUE(selects["A"]->rightAnd->left != NULL);
	ASSERT_TRUE(selects["A"]->rightAnd->rightAnd == NULL);
}
