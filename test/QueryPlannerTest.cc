#include "QueryPlannerTest.h"
#include "OpNode.h"
#include <vector>

QueryPlannerTestVisitor::QueryPlannerTestVisitor(vector<OpNode*> &_nodes): nodes(_nodes) {}

QueryPlannerTestVisitor::~QueryPlannerTestVisitor(){}

void QueryPlannerTestVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitProjectNode(ProjectNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitJoinNode(JoinNode *node, void* arg) {
	node->leftChild->Visit(*this, NULL);
	nodes.push_back(node);
	node->rightChild->Visit(*this, NULL);
}

void QueryPlannerTestVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitSumNode(SumNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}

void QueryPlannerTestVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {
	node->child->Visit(*this, NULL);
	nodes.push_back(node);
}
