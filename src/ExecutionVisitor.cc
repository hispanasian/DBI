#include "ExecutionVisitor.h"

ExecutionVisitor::ExecutionVisitor(){}
ExecutionVisitor::~ExecutionVisitor(){}

void ExecutionVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg){}
void ExecutionVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg){}
void ExecutionVisitor::VisitProjectNode(ProjectNode *node, void* arg){}
void ExecutionVisitor::VisitJoinNode(JoinNode *node, void* arg){}
void ExecutionVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg){}
void ExecutionVisitor::VisitSumNode(SumNode *node, void* arg){}
void ExecutionVisitor::VisitGroupByNode(GroupByNode *node, void* arg){}
void ExecutionVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg){}