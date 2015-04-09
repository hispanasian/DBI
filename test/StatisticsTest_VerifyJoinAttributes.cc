#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * VerifyJoinAttributes should return true if the attributes in the CNF all pertain to the relations
 * provided by relName.
 */
TEST_F(StatisticsTest, VerifyJoinAttributes1) {

    char *relName[] = { "part",  "lineitem"};

	stat.AddRel(relName[0],200000);
	stat.AddAtt(relName[0], "p_partkey",200000);
	stat.AddAtt(relName[0], "p_container",40);

	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "l_partkey",200000);
	stat.AddAtt(relName[1], "l_shipinstruct",4);
	stat.AddAtt(relName[1], "l_shipmode",7);

	char *cnf = "(l_partkey = p_partkey) AND (l_shipmode = 'AIR' OR l_shipmode = 'AIR REG') AND (p_container ='SM BOX' OR p_container = 'SM PACK')  AND (l_shipinstruct = 'DELIVER IN PERSON')";
	yy_scan_string(cnf);
	yyparse();

	EXPECT_EQ(true, stat.VerifyJoinAttributes(final, relName, 2));
}

/**
 * CopyRel should return false the attributes from the cnf do not pertain to a relation in relName
 */
TEST_F(StatisticsTest, VerifyJoinAttributes2) {
    char *relName[] = {"supplier"};

	stat.AddRel(relName[0],10000);
	stat.AddAtt(relName[0], "s_suppkey",10000);

	stat.AddRel(relName[1],800000);
	stat.AddAtt(relName[1], "ps_suppkey", 10000);

	char *cnf = "(s_suppkey = ps_suppkey)";
	yy_scan_string(cnf);
	yyparse();

	EXPECT_EQ(false, stat.VerifyJoinAttributes(final, relName, 1));
}
