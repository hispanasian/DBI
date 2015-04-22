#include "StatisticsTest.h"

TEST_F(StatisticsTest, Estimate1) {
	const char *relName[] = {"supplier","partsupp"};


	stat.AddRel(relName[0],10000);
	stat.AddAtt(relName[0], "s_suppkey",10000);

	stat.AddRel(relName[1],800000);
	stat.AddAtt(relName[1], "ps_suppkey", 10000);

	char *cnf = "(s_suppkey = ps_suppkey)";

	yy_scan_string(cnf);
	yyparse();
	double result = stat.Estimate(final, relName, 2);
	EXPECT_EQ(800000, result);
	stat.Apply(final, relName, 2);

	//reload the statistics object from file
	cnf = "(s_suppkey>1000)";
	yy_scan_string(cnf);
	yyparse();
	double dummy = stat.Estimate(final, relName, 2);
	EXPECT_NEAR(dummy*3.0, result, .1);
}

TEST_F(StatisticsTest, Estimate2) {
	const char *relName[] = {"lineitem"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "l_returnflag",3);
	stat.AddAtt(relName[0], "l_discount",11);
	stat.AddAtt(relName[0], "l_shipmode",7);


	char *cnf = "(l_returnflag = 'R') AND (l_discount < 0.04 OR l_shipmode = 'MAIL')";

	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, relName, 1);
	EXPECT_NEAR(857320.0, result, 10);
}

TEST_F(StatisticsTest, Estimate3) {
	const char *relName[] = {"orders","customer","nation"};


	stat.AddRel(relName[0],1500000);
	stat.AddAtt(relName[0], "o_custkey",150000);

	stat.AddRel(relName[1],150000);
	stat.AddAtt(relName[1], "c_custkey",150000);
	stat.AddAtt(relName[1], "c_nationkey",25);

	stat.AddRel(relName[2],25);
	stat.AddAtt(relName[2], "n_nationkey",25);

	char *cnf = "(c_custkey = o_custkey)";
	yy_scan_string(cnf);
	yyparse();

	// Join the first two relations in relName
	stat.Apply(final, relName, 2);

	cnf = " (c_nationkey = n_nationkey)";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, relName, 3);
	EXPECT_NEAR(1500000.0, result, .1);
}

TEST_F(StatisticsTest, Estimate4) {
	const char *relName[] = {"supplier","customer","nation"};

	stat.AddRel(relName[0],10000);
	stat.AddAtt(relName[0], "s_nationkey",25);

	stat.AddRel(relName[1],150000);
	stat.AddAtt(relName[1], "c_custkey",150000);
	stat.AddAtt(relName[1], "c_nationkey",25);

	stat.AddRel(relName[2],25);
	stat.AddAtt(relName[2], "n_nationkey",25);

	stat.CopyRel("nation","n1");
	stat.CopyRel("nation","n2");
	stat.CopyRel("supplier","s");
	stat.CopyRel("customer","c");

	const char *set1[] ={"s","n1"};
	char *cnf = "(s.s_nationkey = n1.n_nationkey)";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, set1, 2);

	const char *set2[] ={"c","n2"};
	cnf = "(c.c_nationkey = n2.n_nationkey)";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, set2, 2);

	const char *set3[] = {"c","s","n1","n2"};
	cnf = " (n1.n_nationkey = n2.n_nationkey )";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, set3, 4);
	EXPECT_NEAR(60000000.0, result, .1);
}

TEST_F(StatisticsTest, Estimate5) {
	const char *relName[] = { "part", "partsupp", "supplier", "nation", "region"};

	stat.AddRel(relName[0],200000);
	stat.AddAtt(relName[0], "p_partkey",200000);
	stat.AddAtt(relName[0], "p_size",50);

	stat.AddRel(relName[1], 800000);
	stat.AddAtt(relName[1], "ps_suppkey",10000);
	stat.AddAtt(relName[1], "ps_partkey", 200000);

	stat.AddRel(relName[2],10000);
	stat.AddAtt(relName[2], "s_suppkey",10000);
	stat.AddAtt(relName[2], "s_nationkey",25);

	stat.AddRel(relName[3],25);
	stat.AddAtt(relName[3], "n_nationkey",25);
	stat.AddAtt(relName[3], "n_regionkey",5);

	stat.AddRel(relName[4],5);
	stat.AddAtt(relName[4], "r_regionkey",5);
	stat.AddAtt(relName[4], "r_name",5);

	stat.CopyRel("part","p");
	stat.CopyRel("partsupp","ps");
	stat.CopyRel("supplier","s");
	stat.CopyRel("nation","n");
	stat.CopyRel("region","r");

	const char *shortRelName[] = { "p", "ps", "s", "n", "r"};

	char *cnf = "(p.p_partkey=ps.ps_partkey) AND (p.p_size = 2)";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, shortRelName, 2);

	cnf ="(s.s_suppkey = ps.ps_suppkey)";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, shortRelName, 3);

	cnf =" (s.s_nationkey = n.n_nationkey)";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, shortRelName, 4);

	cnf ="(n.n_regionkey = r.r_regionkey) AND (r.r_name = 'AMERICA') ";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, shortRelName, 5);
	EXPECT_NEAR(3200.0, result, .1);
}

TEST_F(StatisticsTest, Estimate6) {
	const char *relName[] = { "customer", "orders", "lineitem"};

	stat.AddRel(relName[0],150000);
	stat.AddAtt(relName[0], "c_custkey",150000);
	stat.AddAtt(relName[0], "c_mktsegment",5);

	stat.AddRel(relName[1],1500000);
	stat.AddAtt(relName[1], "o_orderkey",1500000);
	stat.AddAtt(relName[1], "o_custkey",150000);
	stat.AddAtt(relName[1], "o_orderdate",150000);

	stat.AddRel(relName[2],6001215);
	stat.AddAtt(relName[2], "l_orderkey",1500000);


	char *cnf = "(c_mktsegment = 'BUILDING')  AND (c_custkey = o_custkey)  AND (o_orderdate < '1995-03-1')";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, relName, 2);


	cnf = " (l_orderkey = o_orderkey) ";
	yy_scan_string(cnf);
	yyparse();


	double result = stat.Estimate(final, relName, 3);
	EXPECT_NEAR(400081.0, result, .1);
}

TEST_F(StatisticsTest, Estimate7) {
	const char *relName[] = { "partsupp", "supplier", "nation"};

	stat.AddRel(relName[0],800000);
	stat.AddAtt(relName[0], "ps_suppkey",10000);

	stat.AddRel(relName[1],10000);
	stat.AddAtt(relName[1], "s_suppkey",10000);
	stat.AddAtt(relName[1], "s_nationkey",25);

	stat.AddRel(relName[2],25);
	stat.AddAtt(relName[2], "n_nationkey",25);
	stat.AddAtt(relName[2], "n_name",25);


	char *cnf = " (s_suppkey = ps_suppkey) ";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, relName, 2);

	cnf = " (s_nationkey = n_nationkey)  AND (n_name = 'AMERICA')   ";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, relName, 3);
	EXPECT_NEAR(32000.0, result, .1);
}

TEST_F(StatisticsTest, Estimate8) {
	const char *relName[] = { "orders", "lineitem"};

	stat.AddRel(relName[0],1500000);
	stat.AddAtt(relName[0], "o_orderkey",1500000);


	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "l_orderkey",1500000);
	stat.AddAtt(relName[1], "l_receiptdate",-1);


	char *cnf = "(l_receiptdate >'1995-02-01' ) AND (l_orderkey = o_orderkey)";

	yy_scan_string(cnf);
	yyparse();
	double result = stat.Estimate(final, relName, 2);
	EXPECT_NEAR(2000405.0, result, .1);
}

TEST_F(StatisticsTest, Estimate9) {
	const char *relName[] = { "part",  "partsupp"};

	stat.AddRel(relName[0],200000);
	stat.AddAtt(relName[0], "p_partkey",200000);
	stat.AddAtt(relName[0], "p_size",50);

	stat.AddRel(relName[1],800000);
	stat.AddAtt(relName[1], "ps_partkey",200000);


	char *cnf = "(p_partkey=ps_partkey) AND (p_size =3 OR p_size=6 OR p_size =19)";

	yy_scan_string(cnf);
	yyparse();


	double result = stat.Estimate(final, relName,2);
	EXPECT_NEAR(48000.0, result, .1);
}

TEST_F(StatisticsTest, Estimate10) {
	const char *relName[] = { "part",  "partsupp","supplier"};


	stat.AddRel(relName[0],200000);
	stat.AddAtt(relName[0], "p_partkey",200000);
	stat.AddAtt(relName[0], "p_name", 199996);

	stat.AddRel(relName[1],800000);
	stat.AddAtt(relName[1], "ps_partkey",200000);
	stat.AddAtt(relName[1], "ps_suppkey",10000);

	stat.AddRel(relName[2],10000);
	stat.AddAtt(relName[2], "s_suppkey",10000);

	char *cnf = "(p_partkey=ps_partkey) AND (p_name = 'dark green antique puff wheat') ";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, relName,2);

	cnf = " (s_suppkey = ps_suppkey) ";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, relName,3);
	EXPECT_NEAR(4.0, result, .5);
}

TEST_F(StatisticsTest, Estimate11) {
	const char *relName[] = { "customer", "orders", "lineitem","nation"};

	stat.AddRel(relName[0],150000);
	stat.AddAtt(relName[0], "c_custkey",150000);
	stat.AddAtt(relName[0], "c_nationkey",25);

	stat.AddRel(relName[1],1500000);
	stat.AddAtt(relName[1], "o_orderkey",1500000);
	stat.AddAtt(relName[1], "o_custkey",150000);
	stat.AddAtt(relName[1], "o_orderdate",-1);

	stat.AddRel(relName[2],6001215);
	stat.AddAtt(relName[2], "l_orderkey",1500000);

	stat.AddRel(relName[3],25);
	stat.AddAtt(relName[3], "n_nationkey",25);

	char *cnf = "(c_custkey = o_custkey)  AND (o_orderdate > '1994-01-23') ";
	yy_scan_string(cnf);
	yyparse();
	stat.Apply(final, relName, 2);

	cnf = " (l_orderkey = o_orderkey) ";
	yy_scan_string(cnf);
	yyparse();

	stat.Apply(final, relName, 3);

	cnf = "(c_nationkey = n_nationkey) ";
	yy_scan_string(cnf);
	yyparse();

	double result = stat.Estimate(final, relName, 4);
	EXPECT_NEAR(2000405.0, result, .1);
}

TEST_F(StatisticsTest, Estimate12) {
	Statistics s;
	const char *relName[] = { "part",  "lineitem"};

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

	double result = stat.Estimate(final, relName,2);

	EXPECT_NEAR(21432.9, result, .5);
}

TEST_F(StatisticsTest, Estimate13) {
    Statistics stats;
    stats.AddRel("A", 100);
    stats.AddAtt("A", "y", 10);
    // char* cnf1 = "";
    // yy_scan_string(cnf1);
    // yyparse();
    AndList* and1 = new AndList{NULL, NULL};// = final;
    const char* rels[] = {"A"};
    // cout << (&and1 == NULL) << endl;
    EXPECT_EQ(100, stats.Estimate(and1, rels, 1));
}