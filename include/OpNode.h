/*
 * OpNode.h
 *
 *  Created on: Apr 18, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_OPNODE_H_
#define INCLUDE_OPNODE_H_

#include "Statistics.h"
#include "SQL.h"
#include "Schema.h"
#include "RelationalOp.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "Join.h"
#include "DuplicateRemoval.h"
#include "Sum.h"
#include "GroupBy.h"
#include "WriteOut.h"
#include <vector>

typedef std::vector<Pipe*> Pipes;

class OpVisitor;

/**
 * OpNode is a tree based structure that represents the execution plan of a query. Each OpNode
 * will be given a unique id which it can also use to access its output pipe.
 *
 * Furthermore, OpNode will support the visitor pattern which will be used to perform the work
 * on the OpNode tree.
 */
class OpNode {
protected:
	Schema schema;		// The schema of the Record that will be produced by this OpNode
	int id;				// This OpNode's unique ID
	bool schemaReady;	// Flag telling if the schema has been calculated or not

public:
	OpNode(int _id): id(id) { schemaReady = false; }
	OpNode(int _id, const Schema &_schema): id(id), schema(_schema) { schemaReady = true; }
	virtual ~OpNode();

	/**
	 * Returns this OpNode's ID
	 * @return this OpNode's ID
	 */
	virtual int GetID() = 0;

	/**
	 * Blocks until this node finishes executing.
	 */
	virtual void WaitUntilDone() = 0;

	/**
	 * The method that supports the Visitor pattern. This method will take in a visitor and an
	 * argument.
	 * @param visitor	The visitor that will visit this node
	 * @param arg		Some argument
	 */
	virtual void Visit(OpVisitor &visitor, void* arg) = 0;

	/**
	 * Returns a reference to this objects Schema.
	 * @return	A reference to this objects Schema
	 */
	virtual const Schema& GetSchema() = 0;
};

class SelectPipeNode: public OpNode {
public:
	SelectPipe op;
	OpNode *child;

	SelectPipeNode(int id, OpNode *_child);
	virtual ~SelectPipeNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class SelectFileNode: public OpNode {
public:
	SelectFile op;

	SelectFileNode(int id, const Schema &schema);
	virtual ~SelectFileNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class ProjectNode: public OpNode {
public:
	Project op;
	OpNode *child;
	vector<RelAttPair> attsToKeep;

	/**
	 * This method assumes that attsToKeep only provides the attributes that are listed
	 * in the Select clause exclusing the aggregate. Do NOT provide the aggregate as an attribute
	 * to keep.
	 */
	ProjectNode(int id, OpNode *_child, const std::vector<RelAttPair> &_attsToKeep);
	virtual ~ProjectNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class JoinNode: public OpNode {
public:
	Join op;
	OpNode *leftChild;
	OpNode *rightChild;

	JoinNode(int id, OpNode *_leftChild, OpNode *_rightChild);
	virtual ~JoinNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class DuplicateRemovalNode: public OpNode {
public:
	DuplicateRemoval op;
	OpNode *child;

	DuplicateRemovalNode(int id, OpNode *_child);
	virtual ~DuplicateRemovalNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class SumNode: public OpNode {
public:
	Join op;
	OpNode *child;
	struct FuncOperator *func;

	/**
	 * _func should be the FuncOperator that will produce the Function for this Sum
	 */
	SumNode(int id, OpNode *_child, struct FuncOperator *_func);
	virtual ~SumNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class GroupByNode: public OpNode {
public:
	Join op;
	OpNode *child;
	struct FuncOperator *func;

	/**
	 * _func should be the FuncOperator that will produce the Function for this GroupBy
	 */
	GroupByNode(int id, OpNode *_child, struct FuncOperator *_func);
	virtual ~GroupByNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class WriteOutNode: public OpNode {
public:
	Join op;
	OpNode *child;

	WriteOutNode(int id, OpNode *_child);
	virtual ~WriteOutNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema& GetSchema();
};

class OpVisitor {
public:
	OpVisitor();
	virtual ~OpVisitor();

	virtual void VisitSelectPipeNode(SelectPipeNode *node, void* arg) = 0;
	virtual void VisitSelectFileNode(SelectFileNode *node, void* arg) = 0;
	virtual void VisitProjectNode(ProjectNode *node, void* arg) = 0;
	virtual void VisitJoinNode(JoinNode *node, void* arg) = 0;
	virtual void VisitDuplicateRemovalNode(DuplicateRemovalNode *node, void* arg) = 0;
	virtual void VisitSumNode(SumNode *node, void* arg) = 0;
	virtual void VisitGroupByNode(GroupByNode *node, void* arg) = 0;
	virtual void VisitWriteOutNode(WriteOutNode *node, void* arg) = 0;
};

#endif /* INCLUDE_OPNODE_H_ */
