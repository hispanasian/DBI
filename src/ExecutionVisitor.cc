#include "ExecutionVisitor.h"

ExecutionVisitor::ExecutionVisitor(){}
ExecutionVisitor::~ExecutionVisitor(){}

void ExecutionVisitor::VisitSelectPipeNode(SelectPipeNode *node, void* arg){
	node->child->Visit(*this, arg);
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;

}
void ExecutionVisitor::VisitSelectFileNode(SelectFileNode *node, void* arg){
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;
	DBFile* dbfile = new DBFile(); 
	int ret = dbfile->Open(node->fname.c_str());
	cout << node->fname << endl;
	cout << ret << endl;
	data->files.push_back(dbfile);
	Pipe* pipe = new Pipe();
	data->pipes.emplace(node->GetID(), pipe);
	node->op.Run(*dbfile, *pipe, node->cnf, node->literal);
}
void ExecutionVisitor::VisitProjectNode(ProjectNode *node, void* arg){
	node->child->Visit(*this, arg);
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;
	Pipe* pipe = new Pipe();
	data->pipes.emplace(node->GetID(), pipe);
	node->op.Run(*data->pipes.at(node->child->GetID()), *pipe, node->keepMe,
		node->numAttsInput, node->numAttsOutput);
}
void ExecutionVisitor::VisitJoinNode(JoinNode *node, void* arg){}
void ExecutionVisitor::VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg){
	node->child->Visit(*this, arg);
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;
	Pipe* pipe = new Pipe();
	data->pipes.emplace(node->GetID(), pipe);
	Schema& copy = *new Schema(*node->GetSchema());
	Schema& duplicates = *new Schema(copy, node->duplicates);
	node->op.Run(*data->pipes.at(node->child->GetID()), *pipe, duplicates);
}
void ExecutionVisitor::VisitSumNode(SumNode *node, void* arg){
	node->child->Visit(*this, arg);
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;
	Pipe* pipe = new Pipe();
	data->pipes.emplace(node->GetID(), pipe);
	node->op.Run(*data->pipes.at(node->child->GetID()), *pipe, node->function);
}
void ExecutionVisitor::VisitGroupByNode(GroupByNode *node, void* arg){}
void ExecutionVisitor::VisitWriteOutNode(WriteOutNode *node, void* arg){
	node->child->Visit(*this, arg);
	ExecutionVisitorData* data = (ExecutionVisitorData*) arg;
	Pipe* pipe = new Pipe();
	data->pipes.emplace(node->GetID(), pipe);
	Schema& copy = *new Schema(*node->GetSchema());
	node->op.Run(*data->pipes.at(node->child->GetID()), node->outFile, copy);
}