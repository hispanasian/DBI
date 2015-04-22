/*
 * QueryPlanner.cc
 *
 *  Created on: Apr 19, 2015
 *      Author: cvasquez
 */

#include "QueryPlanner.h"
#include "OpNode.h"
#include "SQL.h"
#include "JoinOptimizer.h"
#include <vector>

QueryPlanner::QueryPlanner(): plan(NULL) {}

QueryPlanner::~QueryPlanner() {
	// TODO Auto-generated destructor stub
}

ExecutionPlan* QueryPlanner::GetPlan() const {
	return plan;
}

void QueryPlanner::Plan(const SQL &sql, const RelationData &relData, FILE* outFile) {
	JoinOptimizer opt;
	Plan(sql, relData, outFile, opt);
}

void QueryPlanner::Plan(const SQL &sql, const RelationData &relData, FILE* outFile, JoinOptimizer &opt) {
	// Before making the plan, create some relational data we can use. We need a map that contains
	// the aliases
	RelationData aliasData;
	vector<RelAliasPair> pair;
	sql.GetTables(pair);

	for(int i = 0; i < pair.size(); i++) {
		aliasData.Insert(pair[i].Alias(), relData[pair[i].Relation()]);
	}

	// Make the plan in order
	plan = PlanSelectJoins(sql, aliasData, opt);
	plan = PlanDistinct(sql, plan);
	plan = PlanAggregation(sql, plan);
	plan = PlanProject(sql, plan);
	plan = PlanOutput(sql, plan, outFile);
}

ExecutionPlan* QueryPlanner::PlanSelectJoins(const SQL &sql, const RelationData &relData,
		JoinOptimizer &opt) {
	SelectMap selects;
	JoinMap joins;
	vector<string> rels;
	vector<TupleCount> counts;

	sql.GetWhere(selects, joins);
	opt.Optimize(selects, joins, sql.GetStatistics(), rels, counts);

	int id = 0; // also size
	OpNode *sel = NULL;
	OpNode *join = NULL;
	OpNode *prev = NULL; // previous join
	// counts and rels should be the same size and should have a one to one correspondence
	for(int i = 0; i < counts.size(); i++) {
		// Counts holds the expected tuples produced from the selects/joins of the relations in
		// order from least to greatest. Hence, we can build a left deep tree in order
		sel = new SelectFileNode(id,
				relData[rels[i]].schema,
				selects[rels[i]],
				relData[rels[i]].dbLocation);
		++id;

		if(i == 0) {
			prev = sel;
			join = sel;
		}

		else if(i == 1) {
			join = new JoinNode(id,
					prev, counts[i-1].select,
					sel, counts[i].select,
					joins[rels[i-1]][rels[i]]);
			++id;
			prev = join;
		}
		else {
			join = new JoinNode(id,
					prev, counts[i].join,
					sel, counts[i].select,
					joins[rels[i-1]][rels[i]]);
			++id;
			prev = join;
		}
	}

	return new ExecutionPlan { join, id };
}

/**
 * Deals with the creation of nodes that remove duplicates
 * @param sql		The SQL object that will be used to help build the Selects and Joins
 * @param tree		The tree so far
 */
ExecutionPlan* QueryPlanner::PlanDistinct(const SQL &sql, ExecutionPlan* tree) const {
	vector<RelAttPair> distinctAgg;
	vector<RelAttPair> distinctSelect;

	if(sql.DistinctAggregate() || sql.DistinctSelect()) {
		if(sql.DistinctAggregate()) {
			if(sql.Function() == NULL) throw invalid_argument("Aggregate query was found, but "
					"no aggregate was provided (QueryPlanner::PlanDistinct)");
			sql.GetFunctionAttributes(distinctAgg);
		}
		sql.GetSelect(distinctSelect);

		for(auto it = distinctSelect.begin(); it != distinctSelect.end(); it++) {
			distinctAgg.push_back(*it);
		}

		OpNode *dist = new DuplicateRemovalNode(tree->size,
				tree->tree,
				distinctAgg);
		tree->tree = dist;
		tree->size += 1;
	}
	return tree;
}

/**
 * Deals with the creation of nodes that project attributes
 * @param sql		The SQL object that will be used to help build the Selects and Joins
 * @param tree		The tree so far
 */
ExecutionPlan* QueryPlanner::PlanProject(const SQL &sql, ExecutionPlan* tree) const {
	// Don't even bother projecting a SUM since it only has one attribute: The aggregate
	if(dynamic_cast<SumNode*>(tree->tree) == NULL) {
		vector<RelAttPair> select;
		sql.GetSelect(select);
		OpNode *proj = new ProjectNode(tree->size,
				tree->tree,
				select);

		tree->tree = proj;
		tree->size += 1;
	}
	return tree;
}

/**
 * Deals with the creation of nodes that deal with aggregation
 * @param sql		The SQL object that will be used to help build the Selects and Joins
 * @param tree		The tree so far
 */
ExecutionPlan* QueryPlanner::PlanAggregation(const SQL &sql, ExecutionPlan* tree) const {
	vector<RelAttPair> group;
	OpNode *agg = NULL;

	sql.GetGroup(group);

	if(group.size() == 0) {
		if(sql.Function() != NULL) {
			// Sum
			agg = new SumNode(tree->size,
					tree->tree,
					sql.Function());
		}
	}
	else {
		// GroupBy
		if(sql.Function() == NULL) throw invalid_argument("GROUP BY must also have a SUM (QueryPlanner::PlanAggregation)");
		agg = new GroupByNode(tree->size,
				tree->tree,
				group,
				sql.Function());
	}

	if(agg != NULL) {
		tree->tree = agg;
		tree->size += 1;
	}
	return tree;
}

ExecutionPlan* QueryPlanner::PlanOutput(const SQL &sql, ExecutionPlan* tree, FILE* outFile) const {
	OpNode *out;
	out = new WriteOutNode(tree->size,
			tree->tree,
			outFile);

	tree->tree = out;
	tree->size += 1;
	return tree;
}
