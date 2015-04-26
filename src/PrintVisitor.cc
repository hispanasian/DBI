#include "PrintVisitor.h"

PrintVisitor::PrintVisitor() {}
PrintVisitor::~PrintVisitor() {}
void PrintVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg) {
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << "Select Pipe Op\n";
}
void PrintVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << "Select File Op" << endl;;
	out << "Input file " << node->fname << endl;
	out << "Output pipe ID " << node->GetID() << endl;
	out << "Output Schema:" << endl;
	out << node->GetSchema()->ToString("\t");
	out << "Select File CNF:" << endl;
	out << node->cnf.ToString();
}
void PrintVisitor::VisitProjectNode(ProjectNode *node, void* arg) {}
void PrintVisitor::VisitJoinNode(JoinNode *node, void* arg) {}
void PrintVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {}
void PrintVisitor::VisitSumNode(SumNode *node, void* arg) {}
void PrintVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {}
void PrintVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {}