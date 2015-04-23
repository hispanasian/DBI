#ifndef QUERYPLANNERTEST_H
#define QUERYPLANNERTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "OpNode.h"
#include "Schema.h"
#include "Defs.h"
#include "SQL.h"
#include "Statistics.h"
#include "QueryPlanner.h"
#include <string>

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Assign;

/**
 * Puts the nodes of the tree in order into nodes.
 */
class QueryPlannerTestVisitor: public OpVisitor {
public:
	std::vector<OpNode*> &nodes;
	bool print;

	QueryPlannerTestVisitor(std::vector<OpNode*> &_nodes);
	virtual ~QueryPlannerTestVisitor();

	virtual void VisitSelectPipeNode(SelectPipeNode *node, void* arg);
	virtual void VisitSelectFileNode(SelectFileNode *node, void* arg);
	virtual void VisitProjectNode(ProjectNode *node, void* arg);
	virtual void VisitJoinNode(JoinNode *node, void* arg);
	virtual void VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg);
	virtual void VisitSumNode(SumNode *node, void* arg);
	virtual void VisitGroupByNode(GroupByNode *node, void* arg);
	virtual void VisitWriteOutNode(WriteOutNode *node, void* arg);
};

class QueryPlannerTest: public ::testing::Test {
public:
	Schema A;
	Schema B;
	Schema C;
	Schema D;
	Schema E;

	Attribute *atts = new Attribute[9];
	Attribute *attsA = new Attribute[3];
	Attribute *attsB = new Attribute[2];
	Attribute *attsC = new Attribute[1];
	Attribute *attsD = new Attribute[1];
	Attribute *attsE = new Attribute[2];

	MockJoinOptimizer joinOptimizer;
	RelationData relData;
	Statistics stats;
	std::string select;
	std::string selectDistinct;
	std::string sum;
	std::string sumDistinct;
	std::string groupBy;
	std::string groupByDistinct;
	std::string groupBySum;
	std::string groupBySumDistinct;

	QueryPlannerTest();
	virtual ~QueryPlannerTest();
};

#endif
