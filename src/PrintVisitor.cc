#include "PrintVisitor.h"

PrintVisitor::PrintVisitor() {}
PrintVisitor::~PrintVisitor() {}
void PrintVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----Select Pipe Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  CNF: " << node->cnf.ToString();
	out << endl;
}

void PrintVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << "-----Select File Op-----" << endl;;
	out << "  Input file: " << node->fname << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  CNF: " << node->cnf.ToString();
	out << endl;
}

void PrintVisitor::VisitProjectNode(ProjectNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "Project Op\n";
	out << "Input pipe ID " << node->child->GetID() << endl;
	out << "Output pipe ID " << node->GetID() << endl;
	out << "Output Schema:" << endl;
	out << node->GetSchema()->ToString("\t");
	out << "Atts To Keep:" << endl;
	// out << node->cnf.ToString();
}
void PrintVisitor::VisitJoinNode(JoinNode *node, void* arg) {}
void PrintVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {}
void PrintVisitor::VisitSumNode(SumNode *node, void* arg) {}
void PrintVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {}
void PrintVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {}
