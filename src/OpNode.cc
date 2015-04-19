/*
 * OpNode.cc
 *
 *  Created on: Apr 18, 2015
 *      Author: cvasquez
 */

#include "OpNode.h"
#include "Defs.h"
#include <string>

// OpNode
OpNode::~OpNode() {}


// SelectPipeNode
SelectPipeNode::SelectPipeNode(int id, OpNode *_child): OpNode(id) {
	child = _child;
}

SelectPipeNode::~SelectPipeNode() {}

void SelectPipeNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSelectPipeNode(this, arg);
}

const Schema& SelectPipeNode::GetSchema() {
	if(schemaReady) return schema;

	// Schema has not been obtained. Thankfully, Select does not change the Schema so simply
	// get the schema from the child
	schema = child->GetSchema();

	schemaReady = true;
	return schema;
}


// SelectFileNode
SelectFileNode::SelectFileNode(int id, const Schema &schema): OpNode(id, schema) {

}

SelectFileNode::~SelectFileNode() {}

void SelectFileNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSelectFileNode(this, arg);
}

const Schema& SelectFileNode::GetSchema() {
	// SelectFile always knows its schema
	return schema;
}


// ProjectNode
ProjectNode::ProjectNode(int id, OpNode *_child, const vector<RelAttPair> &_attsToKeep): OpNode(id),
		attsToKeep(_attsToKeep) {
	child = _child;
}

ProjectNode::~ProjectNode() {}

void ProjectNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitProjectNode(this, arg);
}

const Schema& ProjectNode::GetSchema() {
	if(schemaReady) return schema;

	// Project is a little tricky. It heavily depends on if the child was an aggregate function.
	// If the child was an aggregate function, we cannot simply project away all the attributes
	// except for those that we want. We will make the assumption that if the first attribute in
	// the childs' schema is Aggregate, then it must be an aggregate and we will not project it away
	// This method
	const Schema &childsSchema = child->GetSchema();
	if(childsSchema.Find("Aggregate") != -1) {
		// the schema needs to contain the aggregate. modify the attsToKeep to retain it.
		RelAttPair temp ("", "Aggregate");
		RelAttPair prev ("", "");

		for(int i = 0; i < attsToKeep.size(); i++) {
			prev = attsToKeep[i];
			attsToKeep[i] = temp;
			temp = prev;
		}
		attsToKeep.push_back(prev);
	}
	schema = Schema(childsSchema, attsToKeep);

	schemaReady = true;
	return schema;
}


// JoinNode
JoinNode::JoinNode(int id, OpNode *_leftChild, OpNode *_rightChild): OpNode(id) {
	leftChild = _leftChild;
	rightChild = _rightChild;
}

JoinNode::~JoinNode() {}

void JoinNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitJoinNode(this, arg);
}

const Schema& JoinNode::GetSchema() {
	if(schemaReady) return schema;

	// Merge the schemas from joins children to get the new schema. merge left first, then right
	schema = Schema(leftChild->GetSchema(), rightChild->GetSchema());

	schemaReady = true;
	return schema;
}


// DuplicateRemovalNode
DuplicateRemovalNode::DuplicateRemovalNode(int id, OpNode *_child): OpNode(id) {
	child = _child;
}

DuplicateRemovalNode::~DuplicateRemovalNode() {}

void DuplicateRemovalNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitDuplicateRemovalNode(this, arg);
}

const Schema& DuplicateRemovalNode::GetSchema() {
	if(schemaReady) return schema;

	// DuplicateRemoval does not modify the schema (thankfully)
	schema = child->GetSchema();

	schemaReady = true;
	return schema;
}

// SumNode
SumNode::SumNode(int id, OpNode *_child, struct FuncOperator *_func): OpNode(id) {
	child = _child;
	func = _func;
}

SumNode::~SumNode() {}

void SumNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitSumNode(this, arg);
}

const Schema& SumNode::GetSchema() {
	if(schemaReady) return schema;

	// We need to find out if the schema will result in a int aggregate or double aggregate
	Function temp;
	const Schema &childsSchema = child->GetSchema();
	temp.GrowFromParseTree(func, childsSchema);
	Attribute atts[1];
	atts[0].name = "Aggregate";
	atts[0].relation = "";

	if(temp.ReturnsInt()) atts[0].myType = Int;
	else atts[0].myType = Double;
	schema = Schema("", 1, atts);

	schemaReady = true;
	return schema;
}


// GroupByNode
GroupByNode::GroupByNode(int id, OpNode *_child, struct FuncOperator *_func): OpNode(id) {
	child = _child;
	func = _func;
}

GroupByNode::~GroupByNode() {}

void GroupByNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitGroupByNode(this, arg);
}

const Schema& GroupByNode::GetSchema() {
	if(schemaReady) return schema;

	// We need to find out if the schema will result in a int aggregate or double aggregate
	Function temp;
	const Schema &childsSchema = child->GetSchema();
	temp.GrowFromParseTree(func, childsSchema);
	Attribute atts[1];
	atts[0].name = "Aggregate";
	atts[0].relation = "";

	if(temp.ReturnsInt()) atts[0].myType = Int;
	else atts[0].myType = Double;
	Schema agg("", 1, atts);
	schema = Schema(agg, childsSchema);

	schemaReady = true;
	return schema;
}

// WriteOutNode
WriteOutNode::WriteOutNode(int id, OpNode *_child): OpNode(id) {
	child = _child;
}

WriteOutNode::~WriteOutNode() {}

void WriteOutNode::Visit(OpVisitor &visitor, void* arg) {
	visitor.VisitWriteOutNode(this, arg);
}

const Schema& WriteOutNode::GetSchema() {
	if(schemaReady) return schema;

	// Simply return the childs schema
	schema = Schema(child->GetSchema());

	schemaReady = true;
	return schema;
}
