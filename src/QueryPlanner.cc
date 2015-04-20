/*
 * QueryPlanner.cc
 *
 *  Created on: Apr 19, 2015
 *      Author: cvasquez
 */

#include "QueryPlanner.h"

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

}
