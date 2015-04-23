#include "JoinOptimizerTest.h"
#include <algorithm>

/*
   Sets equal properly handles sets which are equal 
 */
TEST_F(JoinOptimizerTest, SetsEqual1) {
    vector<bool> set1;
    set1.push_back(true);
    set1.push_back(false);
    set1.push_back(true);
    vector<bool> set2;
    set2.push_back(true);
    set2.push_back(false);
    set2.push_back(true);

    EXPECT_EQ(true, SetsEqual(set1, set2));
}

/*
   Sets equal properly handles sets which are not equal 
 */
TEST_F(JoinOptimizerTest, SetsEqual2) {
    vector<bool> set1;
    set1.push_back(true);
    set1.push_back(true);
    set1.push_back(true);
    vector<bool> set2;
    set2.push_back(true);
    set2.push_back(false);
    set2.push_back(true);

    EXPECT_EQ(false, SetsEqual(set1, set2));
}

/*
   Sets equal properly handles sets which are of different size
 */
TEST_F(JoinOptimizerTest, SetsEqual3) {
    vector<bool> set1;
    set1.push_back(true);
    set1.push_back(true);
    vector<bool> set2;
    set2.push_back(true);
    set2.push_back(false);
    set2.push_back(true);

    EXPECT_EQ(false, SetsEqual(set1, set2));
}
/*
    Inidces handles a normal case
 */
TEST_F(JoinOptimizerTest, Indices1) {
    vector<bool> set;
    set.push_back(true);
    set.push_back(false);
    set.push_back(true);
    set.push_back(false);

    vector<int> indices;  
    Indices(set, indices);
    EXPECT_EQ(2, indices.size());
    EXPECT_EQ(0, indices[0]);
    EXPECT_EQ(2, indices[1]);
}

/*
   GetAndList should find the appropriate join,
   even if there are multiple for the given relation 
 */
TEST_F(JoinOptimizerTest, GetAndList1) {
    int index = 2;
    vector<int> indices;
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    vector<string> relNames;
    relNames.push_back("A");
    relNames.push_back("B");
    relNames.push_back("C");
    relNames.push_back("D");

    AndList* aAndB = new AndList{NULL, NULL};
    AndList* bAndC = new AndList{NULL, NULL};
    AndList* cAndD = new AndList{NULL, NULL};
    unordered_map<string, AndList*> alist;
    alist.emplace("B", aAndB);
    unordered_map<string, AndList*> blist;
    blist.emplace("A", aAndB);
    blist.emplace("C", bAndC);
    unordered_map<string, AndList*> clist;
    clist.emplace("B", bAndC);
    clist.emplace("D", cAndD);
    unordered_map<string, AndList*> dlist;
    clist.emplace("C", cAndD);
    unordered_map<string, unordered_map<string, AndList*> > joins;
    joins.emplace("A", alist);
    joins.emplace("B", blist);
    joins.emplace("C", clist);
    joins.emplace("D", dlist);

    EXPECT_EQ(cAndD, GetAndList(index, indices, relNames, joins));
}

/*
    GetRelNames functions as it should
 */
TEST_F(JoinOptimizerTest, GetRelNames1) {
    int index = 2;

    vector<int> indices;
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    vector<string> relNames;
    relNames.push_back("A");
    relNames.push_back("B");
    relNames.push_back("C");
    relNames.push_back("D");
    
    const char* names[3]; 
    GetRelNames(index, indices, names, relNames);
    EXPECT_STREQ("C", names[0]);
    EXPECT_STREQ("A", names[1]);
    EXPECT_STREQ("D", names[2]);
}

