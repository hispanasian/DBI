#ifndef PRINTVISTOR_H
#define PRINTVISTOR_H

#include "OpNode.h"

class PrintVisitor: public OpVisitor {
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