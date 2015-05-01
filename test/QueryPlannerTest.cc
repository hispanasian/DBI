#include "QueryPlannerTest.h"
#include "OpNode.h"
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace std;

QueryPlannerTest::~QueryPlannerTest() {}
QueryPlannerTest::QueryPlannerTest() {
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
	// Plan 1
	select = "SELECT A.a, B.a, C.c, D.d, E.b ";
	select.append(" FROM RelA AS A, RelB AS B, RelC AS C, RelD AS D, RelE AS E ");
	select.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a) AND (B.b = C.c) AND (C.c = D.d) AND (D.d = E.e); ");

	// Plan 2
	selectDistinct = "SELECT DISTINCT A.a, B.a, C.c, D.d, E.b ";
	selectDistinct.append(" FROM RelA AS A, RelB AS B, RelC AS C, RelD AS D, RelE AS E ");
	selectDistinct.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a) AND (B.b = C.c) AND (C.c = D.d) AND (D.d = E.e); ");

	// Plan 3
	sum = "SELECT SUM (A.b + 3) ";
	sum.append(" FROM RelA AS A ");
	sum.append(" WHERE (A.b > 0);  ");

	// Plan 4
	sumDistinct = "SELECT SUM DISTINCT (A.b + 3 ) ";
	sumDistinct.append(" FROM RelA AS A ");
	sumDistinct.append(" WHERE (A.b > 0);  ");

	// Plan 5
	groupBy = "SELECT A.a, B.a ";
	groupBy.append(" FROM RelA AS A, RelB AS B ");
	groupBy.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
	groupBy.append(" GROUP BY A.a, B.a ");

	// Plan 6
	groupBySum = "SELECT SUM (A.b + 3 ), A.a, A.c ";
	groupBySum.append(" FROM RelA AS A ");
	groupBySum.append(" WHERE (A.a = 5 OR A.c = 6)  ");
	groupBySum.append(" GROUP BY A.a, A.c ");

	// Plan 7
	groupByDistinct = "SELECT DISTINCT A.a, B.a, A.c ";
	groupByDistinct.append(" FROM RelA AS A, RelB AS B ");
	groupByDistinct.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
	groupByDistinct.append(" GROUP BY A.a, B.a ");

	// Plan 8
	groupBySumDistinct = "SELECT SUM DISTINCT ( A.a + B.a ), A.c ";
	groupBySumDistinct.append(" FROM RelA AS A, RelB AS B ");
	groupBySumDistinct.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.a)  ");
	groupBySumDistinct.append(" GROUP BY A.a, B.a ");
}

QueryPlannerTestVisitor::QueryPlannerTestVisitor(vector<OpNode*> &_nodes): nodes(_nodes), print(false) {}

QueryPlannerTestVisitor::~QueryPlannerTestVisitor(){}

void QueryPlannerTestVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg) {
	if(print) cout << "Visiting SelectPipeNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {
	if(print) cout << "Visiting SelectFileNode " << node->GetID() << endl;;
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitProjectNode(ProjectNode *node, void* arg) {
	if(print) cout << "Visiting ProjectNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitJoinNode(JoinNode *node, void* arg) {
	if(print) cout << "Visiting JoinNode " << node->GetID() << endl;;
	node->leftChild->Visit(*this, NULL);
	nodes.push_back(node);
	node->rightChild->Visit(*this, NULL);
}

void QueryPlannerTestVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {
	if(print) cout << "Visiting DuplicateRemovalNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitSumNode(SumNode *node, void* arg) {
	if(print) cout << "Visiting SumNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {
	if(print) cout << "Visiting GroupByNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {
	if(print) cout << "Visiting WriteOutNode " << node->GetID() << endl;;
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}
