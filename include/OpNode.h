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
	OpNode(): id(0), schemaReady(false) {}
	OpNode(int _id): id(_id), schemaReady(false) {}
	OpNode(int _id, const Schema &_schema): id(_id), schema(_schema), schemaReady(false) {}
	virtual ~OpNode();

	/**
	 * Returns this OpNode's ID
	 * @return this OpNode's ID
	 */
	virtual int GetID();

	/**
	 * Blocks until this node finishes executing.
	 */
	virtual void WaitUntilDone();

	/**
	 * The method that supports the Visitor pattern. This method will take in a visitor and an
	 * argument.
	 * @param visitor	The visitor that will visit this node
	 * @param arg		Some argument
	 */
	virtual void Visit(OpVisitor &visitor, void* arg);

	/**
	 * Returns a reference to this objects Schema.
	 * @return	A reference to this objects Schema
	 */
	virtual const Schema* GetSchema();
};

class SelectPipeNode: public OpNode {
public:
	SelectPipe op;
	OpNode *child;
	CNF cnf;
	Record literal;
	const struct AndList *select;

	/**
	 * _select should be the AndList that this OpNode will filter on
	 */
	SelectPipeNode(int id, OpNode *_child, const struct AndList *_select);
	virtual ~SelectPipeNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class SelectFileNode: public OpNode {
public:
	SelectFile op;
	CNF cnf;
	Record literal;
	const struct AndList *select;

	/**
	 * _select should be the AndList that this OpNode will filter on
	 */
	SelectFileNode(int id, const Schema &schema, const struct AndList *_select);
	virtual ~SelectFileNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class ProjectNode: public OpNode {
public:
	Project op;
	OpNode *child;
	vector<RelAttPair> attsToKeep;
	int *keepMe;

	/**
	 * This method assumes that attsToKeep only provides the attributes that are listed
	 * in the Select clause exclusing the aggregate. Do NOT provide the aggregate as an attribute
	 * to keep.
	 */
	ProjectNode(int id, OpNode *_child, const std::vector<RelAttPair> &_attsToKeep);
	virtual ~ProjectNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema(); // Also modifies attsToKeep
	void WaitUntilDone();
	virtual bool ContainsAggregate();
};

class JoinNode: public OpNode {
public:
	Join op;
	OpNode *leftChild;
	OpNode *rightChild;
	int leftTuples;
	int rightTuples;
	CNF cnf;
	Record literal;
	const struct AndList *join;

	/**
	 * leftTuples and rightTuples are the estimated number of tuples that will be produced from the
	 * corresponding children.
	 */
	JoinNode(int id, OpNode *_leftChild, int _leftTuples, OpNode *_rightChild, int _rightTuples,
			const struct AndList *_join);
	virtual ~JoinNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class DuplicateRemovalNode: public OpNode {
public:
	DuplicateRemoval op;
	OpNode *child;
	const vector<RelAttPair> duplicates;

	/**
	 * duplicates is a vector that contains the duplicates relation/attribute pairs we want to
	 * eliminate
	 */
	DuplicateRemovalNode(int id, OpNode *_child, const std::vector<RelAttPair> &duplicates);
	virtual ~DuplicateRemovalNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class SumNode: public OpNode {
public:
	Join op;
	OpNode *child;
	Function function;
	Record literal;
	const struct FuncOperator *funcOp;

	/**
	 * _func should be the FuncOperator that will produce the Function for this Sum
	 */
	SumNode(int id, OpNode *_child, const struct FuncOperator *_funcOp);
	virtual ~SumNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class GroupByNode: public OpNode {
public:
	Join op;
	OpNode *child;
	Function function;
	const struct FuncOperator *funcOp;

	/**
	 * _func should be the FuncOperator that will produce the Function for this GroupBy
	 */
	GroupByNode(int id, OpNode *_child, const struct FuncOperator *_funcOp);
	virtual ~GroupByNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
};

class WriteOutNode: public OpNode {
public:
	Join op;
	OpNode *child;
	FILE *outFile;

	WriteOutNode(int id, OpNode *_child, FILE *_outFile);
	virtual ~WriteOutNode();
	void Visit(OpVisitor &visitor, void* arg);
	const Schema* GetSchema();
	void WaitUntilDone();
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
