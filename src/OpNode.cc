/*
 * OpNode.cc
 *
 *  Created on: Apr 18, 2015
 *      Author: cvasquez
 */

#include "OpNode.h"
#include "Defs.h"
#include <string>

// OpVisitor
OpVisitor::OpVisitor() {}

OpVisitor::~OpVisitor() {}

// OpNode
OpNode::~OpNode() {}

int OpNode::GetID() {
	return id;
}

void OpNode::WaitUntilDone() {}

void OpNode::Visit(OpVisitor &visitor, void* arg) {}

const Schema* OpNode::GetSchema() {
	return NULL;
}

// SelectPipeNode
SelectPipeNode::SelectPipeNode(int id, OpNode *_child, const struct AndList *_select): OpNode(id) {
	child = _child;
	select = _select;
	GetSchema(); // Make schema
	cnf.GrowFromParseTree(select, &schema, literal);

}

SelectPipeNode::~SelectPipeNode() {}

void SelectPipeNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSelectPipeNode(this, arg);
}

const Schema* SelectPipeNode::GetSchema() {
	if(schemaReady) return &schema;

	// Schema has not been obtained. Thankfully, Select does not change the Schema so simply
	// get the schema from the child
	schema.Copy(*(child->GetSchema()));

	schemaReady = true;
	return &schema;
}

void SelectPipeNode::WaitUntilDone() {
	op.WaitUntilDone();
}


// SelectFileNode
SelectFileNode::SelectFileNode(int id, const Schema &_schema, const struct AndList *_select,
		const string &_fname): OpNode(id, _schema), fname(_fname) {
	select = _select;
	cnf.GrowFromParseTree(select, &schema, literal);
}

SelectFileNode::~SelectFileNode() {}

void SelectFileNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSelectFileNode(this, arg);
}

const Schema* SelectFileNode::GetSchema() {
	// SelectFile always knows its schema
	return &schema;
}

void SelectFileNode::WaitUntilDone() {
	op.WaitUntilDone();
}


// ProjectNode
ProjectNode::ProjectNode(int id, OpNode *_child, const vector<RelAttPair> &_attsToKeep): OpNode(id),
		attsToKeep(_attsToKeep) {
	child = _child;
	keepMe = new int[attsToKeep.size()];
	GetSchema();
}

ProjectNode::~ProjectNode() {
	delete keepMe;
}

void ProjectNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitProjectNode(this, arg);
}

const Schema* ProjectNode::GetSchema() {
	if(schemaReady) return &schema;

	// Project is a little tricky. It heavily depends on if the child was an aggregate function.
	// If the child was an aggregate function, we cannot simply project away all the attributes
	// except for those that we want. We will make the assumption that if the first attribute in
	// the childs' schema is Aggregate, then it must be an aggregate and we will not project it away
	// This method
	const Schema *childsSchema = child->GetSchema();
	if(ContainsAggregate()) {
		// the schema needs to contain the aggregate. modify the attsToKeep to retain it.
		RelAttPair temp ("", "Aggregate");
		RelAttPair prev (temp);

		for(int i = 0; i < attsToKeep.size(); i++) {
			prev.Copy(attsToKeep[i]);
			attsToKeep[i].Copy(temp);
			temp.Copy(prev);
		}
		attsToKeep.push_back(prev);
	}
	numAttsOutput = attsToKeep.size();
	numAttsInput = childsSchema->GetNumAtts();
	for(int i = 0; i < attsToKeep.size(); ++i) {
		int index = childsSchema->Find(attsToKeep[i].Attribute().c_str());	
		if(index == -1) {
			cout << "Could not find attribute " << attsToKeep[i].Attribute() << " in Project";
			throw std::runtime_error("Could not find attribute in Project");
		}
		keepMe[i] = index;
	}
	schema.Filter(*childsSchema, attsToKeep);

	schemaReady = true;
	return &schema;
}

void ProjectNode::WaitUntilDone() {
	op.WaitUntilDone();
}

bool ProjectNode::ContainsAggregate() {
	GroupByNode *group = dynamic_cast<GroupByNode*>(child);
	SumNode *sum = dynamic_cast<SumNode*>(child);
	return (group != NULL || sum != NULL);
}


// JoinNode
JoinNode::JoinNode(int id, OpNode *_leftChild, int _leftTuples, OpNode *_rightChild,
		int _rightTuples, const struct AndList *_join): OpNode(id), leftTuples(_leftTuples),
				rightTuples(_rightTuples), join(_join) {
	// Put the operation that will produce the least amount of tuples as the right child
	if(leftTuples >= rightTuples) {
		leftChild = _leftChild;
		rightChild = _rightChild;
	}
	else {
		leftChild = _rightChild;
		rightChild = _leftChild;
	}

	// Now create cnf
	cnf.GrowFromParseTree(join, leftChild->GetSchema(), rightChild->GetSchema(), literal);
}

JoinNode::~JoinNode() {}

void JoinNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitJoinNode(this, arg);
}

const Schema* JoinNode::GetSchema() {
	if(schemaReady) return &schema;

	// Merge the schemas from joins children to get the new schema. merge left first, then right
	schema.Join(leftChild->GetSchema(), rightChild->GetSchema());

	schemaReady = true;
	return &schema;
}

void JoinNode::WaitUntilDone() {
	op.WaitUntilDone();
}


// DuplicateRemovalNode
DuplicateRemovalNode::DuplicateRemovalNode(int id, OpNode *_child, const vector<RelAttPair> &_duplicates):
		OpNode(id), duplicates(_duplicates) {
	child = _child;
}

DuplicateRemovalNode::~DuplicateRemovalNode() {}

void DuplicateRemovalNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitDuplicateRemovalNode(this, arg);
}

const Schema* DuplicateRemovalNode::GetSchema() {
	if(schemaReady) return &schema;

	// DuplicateRemoval does not modify the schema (thankfully)
	schema.Copy(*(child->GetSchema()));

	schemaReady = true;
	return &schema;
}

void DuplicateRemovalNode::WaitUntilDone() {
	op.WaitUntilDone();
}

// SumNode
SumNode::SumNode(int id, OpNode *_child, const struct FuncOperator *_funcOp): OpNode(id),
		funcOp(_funcOp) {
	child = _child;
	GetSchema(); // Create function
}

SumNode::~SumNode() {}

void SumNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSumNode(this, arg);
}

const Schema* SumNode::GetSchema() {
	if(schemaReady) return &schema;

	// We need to find out if the schema will result in a int aggregate or double aggregate
	const Schema *childsSchema = child->GetSchema();
	function.GrowFromParseTree(funcOp, *childsSchema);
	Attribute atts[1];
	atts[0].name = "Aggregate";
	atts[0].relation = "";

	if(function.ReturnsInt()) atts[0].myType = Int;
	else atts[0].myType = Double;
	Schema copy ("", 1, atts);
	schema.Copy (copy);

	schemaReady = true;
	return &schema;
}

void SumNode::WaitUntilDone() {
	op.WaitUntilDone();
}


// GroupByNode
GroupByNode::GroupByNode(int id, OpNode *_child, const std::vector<RelAttPair> &_group,
		const struct FuncOperator *_funcOp): OpNode(id),funcOp(_funcOp), group(_group) {
	child = _child;
	GetSchema(); // Create function
}

GroupByNode::~GroupByNode() {}

void GroupByNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitGroupByNode(this, arg);
}

const Schema* GroupByNode::GetSchema() {
	if(schemaReady) return &schema;

	// We need to find out if the schema will result in a int aggregate or double aggregate
	const Schema *childsSchema = child->GetSchema();

	if(IsAggregate()) {
		function.GrowFromParseTree(funcOp, *childsSchema);
		Attribute atts[1];
		atts[0].name = "Aggregate";
		atts[0].relation = "";

		if(function.ReturnsInt()) atts[0].myType = Int;
		else atts[0].myType = Double;
		Schema agg("", 1, atts);
		schema.Join(&agg, childsSchema);
	}
	else schema.Copy(*childsSchema);
	// make the OrderMaker
	vector<string> orderAtts;
	for(int i = 0; i < group.size(); ++i) {
		orderAtts.push_back(group[i].Attribute());
	}
	orderMaker = new OrderMaker(childsSchema, &orderAtts);
	orderMaker->Print();


	schemaReady = true;
	return &schema;
}

void GroupByNode::WaitUntilDone() {
	op.WaitUntilDone();
}

bool GroupByNode::IsAggregate() const {
	return funcOp != NULL;
}

// WriteOutNode
WriteOutNode::WriteOutNode(int id, OpNode *_child, FILE *_outFile): OpNode(id) {
	child = _child;
	outFile = _outFile;
}

WriteOutNode::~WriteOutNode() {}

void WriteOutNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitWriteOutNode(this, arg);
}

const Schema* WriteOutNode::GetSchema() {
	if(schemaReady) return &schema;

	// Simply return the childs schema
	schema.Copy(*(child->GetSchema()));

	schemaReady = true;
	return &schema;
}

void WriteOutNode::WaitUntilDone() {
	op.WaitUntilDone();
}
