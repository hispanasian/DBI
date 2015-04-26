#ifndef PRINTVISTOR_H
#define PRINTVISTOR_H

#include "OpNode.h"
#include <iostream>
#include <sstream>
using namespace std;

struct PrintVisitorData {
	stringstream& out;
};

class PrintVisitor: public OpVisitor {
private:
	void ToStringRelAtts(const vector<RelAttPair>& relAtts, stringstream& out); 
	string FuncOperatorToString(const FuncOperator *func); 
public:
	PrintVisitor();
	~PrintVisitor();

	void VisitSelectPipeNode(SelectPipeNode *node, void* arg);
	void VisitSelectFileNode(SelectFileNode *node, void* arg);
	void VisitProjectNode(ProjectNode *node, void* arg);
	void VisitJoinNode(JoinNode *node, void* arg);
	void VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg);
	void VisitSumNode(SumNode *node, void* arg);
	void VisitGroupByNode(GroupByNode *node, void* arg);
	void VisitWriteOutNode(WriteOutNode *node, void* arg);
};
#endif /* PRINTVISTOR_H */