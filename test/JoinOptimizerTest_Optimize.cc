#include "JoinOptimizerTest.h"

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

