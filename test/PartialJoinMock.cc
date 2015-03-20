#include "../include/Join.h"
#include "../include/JoinTest.h"

PartialJoinMock::PartialJoinMock() {

}
PartialJoinMock::~PartialJoinMock() {

}
void PartialJoinMock::Use_n_Pages (int n) {
	Join::Use_n_Pages(n);
}
void PartialJoinMock::Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR) {
	Join::Work(inPipeL, inPipeR, outPipe, selOp, literal, comp, orderL, orderR);
}
void PartialJoinMock::SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR, JoinRelation &relR, JoinRelation &relS, Record &tempL, Record &tempR) {
	Join::SortMergeJoin(pipeL, pipeR, outPipe, orderL, orderR, relR, relS, tempL, tempR);
}
void PartialJoinMock::BlockNestedLoopJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal, JoinRelation &relS, InMemoryRelation &relR) {
	Join::BlockNestedLoopJoin(pipeL, pipeR, outPipe, selOp, literal, relS, relR);
}
