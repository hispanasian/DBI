#ifndef EXECUTIONVISITOR_H 
#define EXECUTIONVISITOR_H 

#include "OpNode.h"

using namespace std;

struct ExecutionVisitorData {
	stringstream& out;
};

class ExecutionVisitor: public OpVisitor {
// private:
// 	void ToStringRelAtts(const vector<RelAttPair>& relAtts, stringstream& out); 
// 	string FuncOperatorToString(const FuncOperator *func); 
public:
	ExecutionVisitor();
	~ExecutionVisitor();

	void VisitSelectPipeNode(SelectPipeNode *node, void* arg);
	void VisitSelectFileNode(SelectFileNode *node, void* arg);
	void VisitProjectNode(ProjectNode *node, void* arg);
	void VisitJoinNode(JoinNode *node, void* arg);
	void VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg);
	void VisitSumNode(SumNode *node, void* arg);
	void VisitGroupByNode(GroupByNode *node, void* arg);
	void VisitWriteOutNode(WriteOutNode *node, void* arg);
};
#endif /*  EXECUTIONVISITOR_H */