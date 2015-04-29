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
	out << "  CNF: " << node->cnf.ToString("");
}

void PrintVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg) {
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << "-----Select File Op-----" << endl;;
	out << "  Input file: " << node->fname << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  CNF: " << endl << node->cnf.ToString("    ");
}

void PrintVisitor::VisitProjectNode(ProjectNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----Project Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  Atts To Keep: " << endl;
	ToStringRelAtts(node->attsToKeep, out);
	out << endl;
}

void PrintVisitor::VisitJoinNode(JoinNode *node, void* arg) {
	node->leftChild->Visit(*this, arg);	
	node->rightChild->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----Join Op-----\n";
	out << "  Left Input pipe ID: " << node->leftChild->GetID() << endl;
	out << "  Right Input pipe ID: " << node->rightChild->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  CNF: " << endl << node->cnf.ToString("    ");
	out << endl;
}

void PrintVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----DuplicateRemoval Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  Duplicate Atts: " << endl;
	ToStringRelAtts(node->duplicates, out);
	out << endl;
}

void PrintVisitor::VisitSumNode(SumNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----Sum Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  Sum Function: " << endl;
	out << "  " << FuncOperatorToString(node->funcOp);
	out << endl;
}

void PrintVisitor::VisitGroupByNode(GroupByNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----Group By Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Output pipe ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
	out << "  Aggregate Function: " << endl;
	out << "  " << FuncOperatorToString(node->funcOp) << endl;
	out << "  OrderMaker: " << endl;
	ToStringRelAtts(node->group, out);	
	out << endl;
}
void PrintVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg) {
	node->child->Visit(*this, arg);	
	PrintVisitorData* data = (PrintVisitorData*) arg;	
	stringstream& out = data->out;
	out << endl;
	out << "-----WriteOut Op-----\n";
	out << "  Input pipe ID: " << node->child->GetID() << endl;
	out << "  Node ID: " << node->GetID() << endl;
	out << "  Output Schema:" << endl;
	out << node->GetSchema()->ToString("    ");
}

void PrintVisitor::ToStringRelAtts(const vector<RelAttPair>& relAtts, stringstream& out) {
	for(int i = 0; i < relAtts.size(); ++i) {
		out << "    " << relAtts[i].Relation() 
			<< (relAtts[i].Relation().compare("") == 0 ? "" : ".")
			<< relAtts[i].Attribute() << endl;
	}
}

string PrintVisitor::FuncOperatorToString(const FuncOperator *func) {
	string temp;
	if(func != NULL) {
		// First, parse left child
		temp.append(" ");
		temp.append(FuncOperatorToString(func->leftOperator));

		// Next, check root.
		struct FuncOperand *op = func->leftOperand;
		if(op != NULL) {
			temp.append(op->value);
			// Print the operator	
		}
		if(func->code == 42) temp.append("*");
		else if(func->code == 43) temp.append("+");
		else if(func->code == 44) temp.append("/");
		else if(func->code == 45) temp.append("-");

		// Finally, check right child
		temp.append(FuncOperatorToString(func->right));
		temp.append(" ");
	}
	return temp;
}