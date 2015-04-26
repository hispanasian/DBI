#include "PrintVisitor.h"

void PrintVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg) {}
void PrintVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {}
void PrintVisitor::VisitProjectNode(ProjectNode *node, void* arg) {}
void PrintVisitor::VisitJoinNode(JoinNode *node, void* arg) {}
void PrintVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {}
void PrintVisitor::VisitSumNode(SumNode *node, void* arg) {}
void PrintVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {}
void PrintVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {}