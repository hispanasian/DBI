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
