#include "JoinOptimizerTest.h"
#include <algorithm>

/*
    If there are zero relations, don't modify rels or counts
 */
TEST_F(JoinOptimizerTest, Optimize1) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;
    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(0, rels.size());
    EXPECT_EQ(0, counts.size());
}

/*
 * If there is one relation, just add the one relation to the output parameters
 */
TEST_F(JoinOptimizerTest, Optimize2) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;

    // set up the info for the one relation
    char *cnf = "(right = 0)";
    yy_scan_string(cnf);
    yyparse();
    selects.emplace("rel", final);
    stats.AddRel("rel", 100);
    stats.AddAtt("rel", "right", 10);


    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(1, rels.size());
    EXPECT_EQ("rel", rels[0]);
    EXPECT_EQ(1, counts.size());
}

/*
 * If there are two relations, the order doesn't matter, but
 * the tuple count should be populated for both.
 */
TEST_F(JoinOptimizerTest, Optimize3) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;

    // set up the info for the one relation
    AndList* left;
    char *cnf = "(left = 0)";
    yy_scan_string(cnf);
    yyparse();
    left = final;
    selects.emplace("rel1", left);
    stats.AddRel("rel1", 100);
    stats.AddAtt("rel1", "left", 10);
    stats.AddRel("rel2", 10);
    stats.AddAtt("rel2", "right", 10);
    AndList* right;
    char *cnf2 = "(right = 0)";
    yy_scan_string(cnf2);
    yyparse();
    right = final;
    selects.emplace("rel2", right);

    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(2, rels.size());
    auto it = find(rels.begin(), rels.end(), "rel1");
    EXPECT_EQ(true, it != rels.end());
    it = find(rels.begin(), rels.end(), "rel2");
    EXPECT_EQ(true, it != rels.end());
    EXPECT_EQ(2, counts.size());
    EXPECT_EQ(2, counts.size());
}

int indexOf(const vector<string>& rels, const string& rel) {
    for(int i = 0; i < rels.size(); ++i) {
        if(rel == rels[i]) return i;
    }
    return -1;
}

/*
 * Three relations 
 */
TEST_F(JoinOptimizerTest, Optimize4) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;

    stats.AddRel("A", 100);
    stats.AddRel("B", 10);
    stats.AddRel("C", 5);

    stats.AddAtt("A", "y", 10);
    stats.AddAtt("B", "y", 10);
    stats.AddAtt("B", "z", 5);
    stats.AddAtt("C", "z", 5);

    // populate selects
    selects.emplace("A", new AndList{NULL, NULL});
    selects.emplace("B", new AndList{NULL, NULL});
    selects.emplace("C", new AndList{NULL, NULL});

    unordered_map<string, AndList*> alist;
    unordered_map<string, AndList*> blist;
    unordered_map<string, AndList*> clist;

    char* cnf1 = "(A.y = B.y)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    alist.emplace("B", and1);
    blist.emplace("A", and1);

    char* cnf2 = "(B.z = C.z)";
    yy_scan_string(cnf2);
    yyparse();
    AndList* and2 = final;
    blist.emplace("C", and2);
    clist.emplace("B", and2);

    joins.emplace("A", alist);
    joins.emplace("B", blist);
    joins.emplace("C", clist);


    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(3, rels.size());
    int bIndex = indexOf(rels, "B");
    int cIndex = indexOf(rels, "C");
    int aIndex = indexOf(rels, "A");
    // B or C can be first, doesn't matter
    EXPECT_EQ(true, bIndex == 0 || bIndex == 1);
    EXPECT_EQ(true, cIndex == 0 || cIndex == 1);
    // A must be 3rd in the list
    EXPECT_EQ(2, aIndex);
    EXPECT_EQ(3, counts.size());
    // Ensure the counts of the selects are correct
    EXPECT_EQ(100, counts[aIndex].select);
    EXPECT_EQ(10, counts[bIndex].select);
    EXPECT_EQ(5, counts[cIndex].select);

    // check size of the joins
    EXPECT_EQ(10, counts[aIndex].join);
}

TEST_F(JoinOptimizerTest, Optimize5) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;

    stats.AddRel("A", 100);
    stats.AddRel("B", 30);
    stats.AddRel("C", 10);
    stats.AddRel("D", 40);

    stats.AddAtt("A", "x", 30);
    stats.AddAtt("B", "x", 30);
    stats.AddAtt("B", "y", 10);
    stats.AddAtt("C", "y", 10);
    stats.AddAtt("C", "z", 40);
    stats.AddAtt("D", "z", 40);

    // populate selects
    selects.emplace("A", new AndList{NULL, NULL});
    selects.emplace("B", new AndList{NULL, NULL});
    selects.emplace("C", new AndList{NULL, NULL});
    selects.emplace("D", new AndList{NULL, NULL});

    unordered_map<string, AndList*> alist;
    unordered_map<string, AndList*> blist;
    unordered_map<string, AndList*> clist;
    unordered_map<string, AndList*> dlist;

    char* cnf1 = "(A.x = B.x)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    alist.emplace("B", and1);
    blist.emplace("A", and1);

    char* cnf2 = "(B.y = C.y)";
    yy_scan_string(cnf2);
    yyparse();
    AndList* and2 = final;
    blist.emplace("C", and2);
    clist.emplace("B", and2);

    char* cnf3 = "(C.z = D.z)";
    yy_scan_string(cnf3);
    yyparse();
    AndList* and3 = final;
    clist.emplace("D", and3);
    dlist.emplace("C", and3);

    joins.emplace("A", alist);
    joins.emplace("B", blist);
    joins.emplace("C", clist);
    joins.emplace("D", dlist);


    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(4, rels.size());
    int cIndex = indexOf(rels, "C");
    int dIndex = indexOf(rels, "D");
    int bIndex = indexOf(rels, "B");
    int aIndex = indexOf(rels, "A");
    // C or D can be first, doesn't matter
    EXPECT_EQ(true, cIndex == 0 || cIndex == 1);
    EXPECT_EQ(true, dIndex == 0 || dIndex == 1);
    // B must be 3rd in the list
    EXPECT_EQ(2, bIndex);
    // A must be 4th in the list
    EXPECT_EQ(3, aIndex);
    EXPECT_EQ(4, counts.size());
    // Ensure the counts of the selects are correct
    EXPECT_EQ(100, counts[aIndex].select);
    EXPECT_EQ(30, counts[bIndex].select);
    EXPECT_EQ(10, counts[cIndex].select);
    EXPECT_EQ(40, counts[dIndex].select);

    // check the sizes of the joins
    EXPECT_EQ(30, counts[aIndex].join);
    EXPECT_EQ(10, counts[bIndex].join);
}

TEST_F(JoinOptimizerTest, Optimize6) {
    unordered_map<string, AndList*> selects;
    unordered_map<string, unordered_map<string, AndList*> > joins;
    Statistics stats;
    vector<string> rels;
    vector<TupleCount> counts;

    stats.AddRel("A", 20);
    stats.AddRel("B", 30);
    stats.AddRel("C", 100);
    stats.AddRel("D", 10);

    stats.AddAtt("A", "x", 20);
    stats.AddAtt("B", "x", 20);
    stats.AddAtt("B", "y", 30);
    stats.AddAtt("C", "y", 30);
    stats.AddAtt("C", "z", 10);
    stats.AddAtt("D", "z", 10);

    // populate selects
    selects.emplace("A", new AndList{NULL, NULL});
    selects.emplace("B", new AndList{NULL, NULL});
    selects.emplace("C", new AndList{NULL, NULL});
    selects.emplace("D", new AndList{NULL, NULL});

    unordered_map<string, AndList*> alist;
    unordered_map<string, AndList*> blist;
    unordered_map<string, AndList*> clist;
    unordered_map<string, AndList*> dlist;

    char* cnf1 = "(A.x = B.x)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    alist.emplace("B", and1);
    blist.emplace("A", and1);

    char* cnf2 = "(B.y = C.y)";
    yy_scan_string(cnf2);
    yyparse();
    AndList* and2 = final;
    blist.emplace("C", and2);
    clist.emplace("B", and2);

    char* cnf3 = "(C.z = D.z)";
    yy_scan_string(cnf3);
    yyparse();
    AndList* and3 = final;
    clist.emplace("D", and3);
    dlist.emplace("C", and3);

    joins.emplace("A", alist);
    joins.emplace("B", blist);
    joins.emplace("C", clist);
    joins.emplace("D", dlist);


    JoinOptimizer opt;
    opt.Optimize(selects, joins, stats, rels, counts);
    EXPECT_EQ(4, rels.size());
    int aIndex = indexOf(rels, "A");
    int bIndex = indexOf(rels, "B");
    int cIndex = indexOf(rels, "C");
    int dIndex = indexOf(rels, "D");
    // C or D can be first, doesn't matter
    EXPECT_EQ(true, aIndex == 0 || aIndex == 1);
    EXPECT_EQ(true, bIndex == 0 || bIndex == 1);
    // B must be 3rd in the list
    EXPECT_EQ(2, cIndex);
    // A must be 4th in the list
    EXPECT_EQ(3, dIndex);

    EXPECT_EQ(4, counts.size());
    // Ensure the counts of the selects are correct
    EXPECT_EQ(20, counts[aIndex].select);
    EXPECT_EQ(30, counts[bIndex].select);
    EXPECT_EQ(100, counts[cIndex].select);
    EXPECT_EQ(10, counts[dIndex].select);

    // check the sizes of the joins
    EXPECT_EQ(30, counts[cIndex].join);
    EXPECT_EQ(100, counts[dIndex].join);
}

// Test with 3 relations
TEST_F(JoinOptimizerTest, Verify1) {
    Statistics stats;
    const char* rels[] = {"A", "B", "C"};
    stats.AddRel("A", 100);
    stats.AddRel("B", 10);
    stats.AddRel("C", 5);

    stats.AddAtt("A", "a", 10);
    stats.AddAtt("B", "a", 10);
    stats.AddAtt("B", "b", 5);
    stats.AddAtt("C", "b", 5);

    char* cnf1 = "(A.a = B.a)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    const char* rels1[] = {"A","B"};
    EXPECT_DOUBLE_EQ(100, stats.Estimate(and1, rels1, 2));

    char* cnf2 = "(B.b = C.b)";
    yy_scan_string(cnf2);
    yyparse();
    const char* rels2[] = {"B", "C"};
    AndList* and2 = final;
    EXPECT_EQ(10, stats.Estimate(and2, rels2, 2));

    Statistics stats1 = stats;
    Statistics stats2 = stats;
    stats1.Apply(and1, rels1, 2);
    stats2.Apply(and2, rels2, 2);
    EXPECT_EQ(100, stats1.Estimate(and2, rels, 3));
    EXPECT_EQ(100, stats2.Estimate(and1, rels, 3));
}

TEST_F(JoinOptimizerTest, Verify2) {
    Statistics stats;
    stats.AddRel("A", 100);
    stats.AddRel("B", 30);
    stats.AddRel("C", 10);
    stats.AddRel("D", 40);

    stats.AddAtt("A", "x", 30);
    stats.AddAtt("B", "x", 30);
    stats.AddAtt("B", "y", 10);
    stats.AddAtt("C", "y", 10);
    stats.AddAtt("C", "z", 40);
    stats.AddAtt("D", "z", 40);

    char* cnf1 = "(A.x = B.x)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    const char* rels1[] = {"A","B"};
    EXPECT_DOUBLE_EQ(100, stats.Estimate(and1, rels1, 2));

    char* cnf2 = "(B.y = C.y)";
    yy_scan_string(cnf2);
    yyparse();
    const char* rels2[] = {"B", "C"};
    AndList* and2 = final;
    EXPECT_EQ(30, stats.Estimate(and2, rels2, 2));

    char* cnf3 = "(C.z = D.z)";
    yy_scan_string(cnf3);
    yyparse();
    const char* rels3[] = {"C", "D"};
    AndList* and3 = final;
    EXPECT_EQ(10, stats.Estimate(and3, rels3, 2));

    Statistics stats1 = stats; // AB
    Statistics stats2 = stats; // BC
    Statistics stats3 = stats; // CD
    stats1.Apply(and1, rels1, 2);
    stats2.Apply(and2, rels2, 2);
    stats3.Apply(and3, rels3, 2);

    const char* rels4[] = {"A", "B", "C"};
    // (AB)C
    EXPECT_EQ(100, stats1.Estimate(and2, rels4, 3));
    // (BC)A
    EXPECT_EQ(100, stats2.Estimate(and1, rels4, 3));
    const char* rels5[] = {"B", "C", "D"};
    // (BC)D
    EXPECT_EQ(30, stats2.Estimate(and3, rels5, 3));
    // (CD)B
    EXPECT_EQ(30, stats3.Estimate(and2, rels5, 3));
    Statistics stats4 = stats1; // ABC
    stats4.Apply(and2, rels4, 3);
    Statistics stats5 = stats2; // BCA
    stats5.Apply(and1, rels4, 3);
    Statistics stats6 = stats2; // BCD
    stats6.Apply(and3, rels5, 3);
    Statistics stats7 = stats3; // CDB
    stats7.Apply(and2, rels5, 3);

    const char* rels6[] = {"A", "B", "C", "D"};
    // ABCD
    EXPECT_EQ(100, stats4.Estimate(and3, rels6, 4));
    // BCAD
    EXPECT_EQ(100, stats5.Estimate(and3, rels6, 4));
    // BCDA 
    EXPECT_EQ(100, stats6.Estimate(and1, rels6, 4));
    // CDBA 
    EXPECT_EQ(100, stats7.Estimate(and1, rels6, 4));
}

TEST_F(JoinOptimizerTest, Verify3) {
    Statistics stats;
    stats.AddRel("A", 20);
    stats.AddRel("B", 30);
    stats.AddRel("C", 100);
    stats.AddRel("D", 10);

    stats.AddAtt("A", "x", 20);
    stats.AddAtt("B", "x", 20);
    stats.AddAtt("B", "y", 30);
    stats.AddAtt("C", "y", 30);
    stats.AddAtt("C", "z", 10);
    stats.AddAtt("D", "z", 10);

    char* cnf1 = "(A.x = B.x)";
    yy_scan_string(cnf1);
    yyparse();
    AndList* and1 = final;
    const char* rels1[] = {"A","B"};
    EXPECT_DOUBLE_EQ(30, stats.Estimate(and1, rels1, 2));

    char* cnf2 = "(B.y = C.y)";
    yy_scan_string(cnf2);
    yyparse();
    const char* rels2[] = {"B", "C"};
    AndList* and2 = final;
    EXPECT_EQ(100, stats.Estimate(and2, rels2, 2));

    char* cnf3 = "(C.z = D.z)";
    yy_scan_string(cnf3);
    yyparse();
    const char* rels3[] = {"C", "D"};
    AndList* and3 = final;
    EXPECT_EQ(100, stats.Estimate(and3, rels3, 2));

    Statistics stats1 = stats; // AB
    Statistics stats2 = stats; // BC
    Statistics stats3 = stats; // CD
    stats1.Apply(and1, rels1, 2);
    stats2.Apply(and2, rels2, 2);
    stats3.Apply(and3, rels3, 2);

    const char* rels4[] = {"A", "B", "C"};
    // (AB)C
    EXPECT_EQ(100, stats1.Estimate(and2, rels4, 3));
    // (BC)A
    EXPECT_EQ(100, stats2.Estimate(and1, rels4, 3));
    const char* rels5[] = {"B", "C", "D"};
    // (BC)D
    EXPECT_EQ(100, stats2.Estimate(and3, rels5, 3));
    // (CD)B
    EXPECT_EQ(100, stats3.Estimate(and2, rels5, 3));
    Statistics stats4 = stats1; // ABC
    stats4.Apply(and2, rels4, 3);
    Statistics stats5 = stats2; // BCA
    stats5.Apply(and1, rels4, 3);
    Statistics stats6 = stats2; // BCD
    stats6.Apply(and3, rels5, 3);
    Statistics stats7 = stats3; // CDB
    stats7.Apply(and2, rels5, 3);

    const char* rels6[] = {"A", "B", "C", "D"};
    // ABCD
    EXPECT_EQ(100, stats4.Estimate(and3, rels6, 4));
    // BCAD
    EXPECT_EQ(100, stats5.Estimate(and3, rels6, 4));
    // BCDA 
    EXPECT_EQ(100, stats6.Estimate(and1, rels6, 4));
    // CDBA 
    EXPECT_EQ(100, stats7.Estimate(and1, rels6, 4));
}
