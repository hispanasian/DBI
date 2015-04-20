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
	QueryPlanner planner;
	std::string select;
	std::string selectDistinct;
	std::string sum;
	std::string sumDistinct;
	std::string groupBy;
	std::string groupByDistinct;
	std::string groupBySum;
	std::string groupBySumDistinct;

	QueryPlannerTest() {
		Attribute *temp;
		// Make Schemas

		temp = &attsA[0]; temp->relation="A"; temp->name="a"; temp->myType=Int;
		temp = &attsA[1]; temp->relation="A"; temp->name="b"; temp->myType=Int;
		temp = &attsA[2]; temp->relation="A"; temp->name="c"; temp->myType=Int;
		A.Copy(Schema("", 3, attsA));

		temp = &attsB[0]; temp->relation="B"; temp->name="a"; temp->myType=Int;
		temp = &attsB[1]; temp->relation="B"; temp->name="b"; temp->myType=Double;
		B.Copy(Schema("", 2, attsB));

		temp = &attsC[0]; temp->relation="C"; temp->name="c"; temp->myType=Double;
		C.Copy(Schema("", 1, attsC));

		temp = &attsD[0]; temp->relation="D"; temp->name="d"; temp->myType=Double;
		D.Copy(Schema("", 1, attsD));

		temp = &attsE[0]; temp->relation="E"; temp->name="e"; temp->myType=Double;
		temp = &attsE[1]; temp->relation="E"; temp->name="b"; temp->myType=String;
		E.Copy(Schema("", 2, attsE));

		// Populate Relation Data
		relData.Insert("RelA", "data/RelA.db", "schemas", A);
		relData.Insert("RelB", "data/RelB.db", "schemas", B);
		relData.Insert("RelC", "data/RelC.db", "schemas", C);
		relData.Insert("RelD", "data/RelD.db", "schemas", D);
		relData.Insert("RelE", "data/RelE.db", "schemas", E);

		// Make Statistics
		stats.AddRel("RelA", 0);
		stats.AddRel("RelB", 0);
		stats.AddRel("RelC", 0);
		stats.AddRel("RelD", 0);
		stats.AddRel("RelE", 0);

		stats.AddAtt("RelA", "a", 0);
		stats.AddAtt("RelA", "b", 0);
		stats.AddAtt("RelA", "c", 0);
		stats.AddAtt("RelB", "a", 0);
		stats.AddAtt("RelB", "b", 0);
		stats.AddAtt("RelC", "c", 0);
		stats.AddAtt("RelD", "d", 0);
		stats.AddAtt("RelE", "e", 0);
		stats.AddAtt("RelE", "b", 0);

		// Make SQL
		select = "SELECT A.a, B.a, C.c, D.d, E.b ";
		select.append(" FROM RelA AS A, RelB AS B, RelC AS C, RelD AS D, RelE AS E ");
		select.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a) AND (B.b = C.c) AND (C.c = D.d) AND (D.d = E.e) ");

		selectDistinct = "SELECT DISTINCT A.a, B.a, C.c, D.d, E.b ";
		selectDistinct.append(" FROM RelA AS A, RelB AS B, RelC AS C, RelD AS D, RelE AS E ");
		selectDistinct.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a) AND (B.b = C.c) AND (C.c = D.d) AND (D.d = E.e) ");

		sum = "SELECT SUM (A.b + 3) ";
		sum.append(" FROM RelA AS A ");
		sum.append(" WHERE (A.b > 0)  ");

		sumDistinct = "SELECT SUM DISTINCT (A.b + 3 ) ";
		sumDistinct.append(" FROM RelA AS A ");
		sum.append(" WHERE (A.b > 0)  ");


		groupBy = "SELECT A.a, B.a ";
		groupBy.append(" FROM RelA AS A, RelB AS B ");
		groupBy.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
		groupBy.append(" GROUP BY A.a, B.a ");

		groupBySum= "SELECT SUM (A.b + 3 ), A.a, A.c ";
		groupBySum.append(" FROM RelA AS A ");
		groupBySum.append(" WHERE (A.a = 5 OR A.c = 6)  ");
		groupBySum.append(" GROUP BY A.a, A.c ");

		groupBy = "SELECT DISTINCT A.a, B.a, A.c ";
		groupBy.append(" FROM RelA AS A, RelB AS B ");
		groupBy.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
		groupBy.append(" GROUP BY A.a, B.a ");

		groupBy = "SELECT DISTINCT SUM( A.a, B.a ), A.c ";
		groupBy.append(" FROM RelA AS A, RelB AS B ");
		groupBy.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
		groupBy.append(" GROUP BY A.a, B.a ");
	}
};

#endif
