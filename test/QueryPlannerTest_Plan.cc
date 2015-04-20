#include "QueryPlannerTest.h"
#include "OpNode.h"
#include "Schema.h"
#include "SQL.h"
#include <string>
#include <vector>

/**
 * QueryPlanner::Plan should handle the case where only one Select node exists (no joins)
 */

/**
 * QueryPlanner::Plan should handle the case where only two select nodes exist (one single join)
 */

/**
 * QueryPlanner::Plan should produce a plan that puts the smallest relations on the right
 */

/**
 * QueryPlanner::Plan should produce a plan for a basic select statement.
 */
TEST_F(QueryPlannerTest, Plan1) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = select;

	// Setup
	rels.push_back("A");rels.push_back("B");rels.push_back("C");rels.push_back("D");rels.push_back("E");
	counts.push_back(TupleCount{ 0, 0 });
	counts.push_back(TupleCount{ 0, 5 });
	counts.push_back(TupleCount{ 10, 6 });
	counts.push_back(TupleCount{ 11, 7 });
	counts.push_back(TupleCount{ 12, 8 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(11, visitor.nodes.size());
	ASSERT_EQ(11, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(B == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelB.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[2]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2])->fname.compare("data/RelA.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[4]) != NULL);
	EXPECT_TRUE(C == *(visitor.nodes[4]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[4])->fname.compare("data/RelC.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[6]) != NULL);
	EXPECT_TRUE(D == *(visitor.nodes[6]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[6])->fname.compare("data/RelD.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[8]) != NULL);
	EXPECT_TRUE(E == *(visitor.nodes[8]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[8])->fname.compare("data/RelE.db") == 0);

	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[1]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[3]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[5]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[7]) != NULL);

	ASSERT_TRUE(dynamic_cast<ProjectNode*>(visitor.nodes[9]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[10]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[10])->outFile == NULL);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic select distinct statement.
 */
TEST_F(QueryPlannerTest, Plan2) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = selectDistinct;

	// Setup
	rels.push_back("A");rels.push_back("B");rels.push_back("C");rels.push_back("D");rels.push_back("E");
	counts.push_back(TupleCount{ 0, 4 });
	counts.push_back(TupleCount{ 0, 5 });
	counts.push_back(TupleCount{ 10, 6 });
	counts.push_back(TupleCount{ 11, 7 });
	counts.push_back(TupleCount{ 12, 8 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(12, visitor.nodes.size());
	ASSERT_EQ(12, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(B == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelB.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[2]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2])->fname.compare("data/RelA.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[4]) != NULL);
	EXPECT_TRUE(C == *(visitor.nodes[4]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[4])->fname.compare("data/RelC.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[6]) != NULL);
	EXPECT_TRUE(D == *(visitor.nodes[6]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[6])->fname.compare("data/RelD.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[8]) != NULL);
	EXPECT_TRUE(E == *(visitor.nodes[8]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[8])->fname.compare("data/RelE.db") == 0);

	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[1]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[3]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[5]) != NULL);
	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[7]) != NULL);

	ASSERT_TRUE(dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[9]) != NULL);
	EXPECT_EQ(5, dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[9])->duplicates.size());
	ASSERT_TRUE(dynamic_cast<ProjectNode*>(visitor.nodes[10]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[11]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[11])->outFile == NULL);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic select sum statement and there is
 * only one relation (select)
 */
TEST_F(QueryPlannerTest, Plan3) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = sum;

	// Setup
	rels.push_back("A");
	counts.push_back(TupleCount{ 0, 4 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(3, visitor.nodes.size());
	ASSERT_EQ(3, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelA.db") == 0);

	ASSERT_TRUE(dynamic_cast<SumNode*>(visitor.nodes[1]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[2]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[2])->outFile == NULL);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic select distinct sum statement and there is
 * only one relation (select)
 */
TEST_F(QueryPlannerTest, Plan4) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = sumDistinct;

	// Setup
	rels.push_back("A");
	counts.push_back(TupleCount{ 0, 4 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(4, visitor.nodes.size());
	ASSERT_EQ(4, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelA.db") == 0);

	ASSERT_TRUE(dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[1]) != NULL);
	EXPECT_EQ(1, dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[1])->duplicates.size());
	ASSERT_TRUE(dynamic_cast<SumNode*>(visitor.nodes[2]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[3]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[3])->outFile == NULL);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic GroiupBy statement with only two relations
 * (two selections)
 */
TEST_F(QueryPlannerTest, Plan5) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = groupBy;

	// Setup
	rels.push_back("A");rels.push_back("B");
	counts.push_back(TupleCount{ 0, 4 });
	counts.push_back(TupleCount{ 0, 5 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);

	ASSERT_THROW(planner.Plan(sql, relData, NULL, joinOptimizer), invalid_argument);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic GroiupBySum statement with only one relation
 * (one selection)
 */
TEST_F(QueryPlannerTest, Plan6) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = groupBySum;

	// Setup
	rels.push_back("A");
	counts.push_back(TupleCount{ 0, 4 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(4, visitor.nodes.size());
	ASSERT_EQ(4, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelA.db") == 0);

	ASSERT_TRUE(dynamic_cast<GroupByNode*>(visitor.nodes[1]) != NULL);
	ASSERT_EQ(2, dynamic_cast<GroupByNode*>(visitor.nodes[1])->group.size());
	ASSERT_TRUE(dynamic_cast<ProjectNode*>(visitor.nodes[2]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[3]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[3])->outFile == NULL);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic GroiupByDistinct statement with only two
 * relations (two selections)
 */
TEST_F(QueryPlannerTest, Plan7) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = groupByDistinct;

	// Setup
	rels.push_back("A");rels.push_back("B");
	counts.push_back(TupleCount{ 0, 5 });
	counts.push_back(TupleCount{ 0, 4 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	ASSERT_THROW(planner.Plan(sql, relData, NULL, joinOptimizer), invalid_argument);
}

/**
 * QueryPlanner::Plan should produce a plan for a basic GroiupByDistinctSum statement with only
 * two relations (two selections)
 */
TEST_F(QueryPlannerTest, Plan8) {
	QueryPlanner planner;
	OpNode *tree;
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;
	vector<OpNode*> nodes;
	SQL sql (stats);
	QueryPlannerTestVisitor visitor(nodes);

	string query = groupBySumDistinct;

	// Setup
	rels.push_back("A");rels.push_back("B");
	counts.push_back(TupleCount{ 0, 5 });
	counts.push_back(TupleCount{ 0, 4 });

	EXPECT_CALL(joinOptimizer, Optimize(_, _, _, _, _)).
			WillRepeatedly(DoAll(SetArgReferee<3>(rels), SetArgReferee<4>(counts)));

	sql.Parse(query);
	sql.GetWhere(selects, joins);
	planner.Plan(sql, relData, NULL, joinOptimizer);
	tree = (planner.GetPlan())->tree;
	tree->Visit(visitor, NULL);

	ASSERT_EQ(7, visitor.nodes.size());
	ASSERT_EQ(7, (planner.GetPlan())->size);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0]) != NULL);
	EXPECT_TRUE(A == *(visitor.nodes[0]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[0])->fname.compare("data/RelA.db") == 0);
	ASSERT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2]) != NULL);
	EXPECT_TRUE(B == *(visitor.nodes[2]->GetSchema()));
	EXPECT_TRUE(dynamic_cast<SelectFileNode*>(visitor.nodes[2])->fname.compare("data/RelB.db") == 0);

	ASSERT_TRUE(dynamic_cast<JoinNode*>(visitor.nodes[1]) != NULL);
	EXPECT_TRUE(Schema(A, B) == *(visitor.nodes[1]->GetSchema()));

	ASSERT_TRUE(dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[3]) != NULL);
	EXPECT_EQ(3, dynamic_cast<DuplicateRemovalNode*>(visitor.nodes[3])->duplicates.size());
	ASSERT_TRUE(dynamic_cast<GroupByNode*>(visitor.nodes[4]) != NULL);
	ASSERT_EQ(2, dynamic_cast<GroupByNode*>(visitor.nodes[4])->group.size());
	ASSERT_TRUE(dynamic_cast<ProjectNode*>(visitor.nodes[5]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[6]) != NULL);
	ASSERT_TRUE(dynamic_cast<WriteOutNode*>(visitor.nodes[6])->outFile == NULL);
}
