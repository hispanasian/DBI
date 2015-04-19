#include "OpNodeTest.h"
#include "OpNode.h"
#include "Schema.h"
#include "SQL.h"

// SelectPipeNode
/**
 * SelectPipeNode::GetSchema should return it's child's schema
 */
TEST_F(OpNodeTest, SelectPipeNode_GetSchema1) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectPipeNode op (0, &child, selects["A"]);

	ASSERT_EQ(9, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(5, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(6, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(7, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * SelectPipeNode:: typical usage
 */
TEST_F(OpNodeTest, SelectPipeNode_GetSchema2) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&A));
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectPipeNode op (0, &child, selects["A"]);

	ASSERT_EQ(3, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
}

/*
 * SelectPipeNode should not fail to get the schema even if it was empty
 */
TEST_F(OpNodeTest, SelectPipeNode_GetSchema3) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&emptySchema));
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectPipeNode op (0, &child, selects["B"]);

	ASSERT_EQ(0, op.GetSchema()->GetNumAtts());
}

// SelectFileNode
/**
 * SelectFileNode::GetSchema should return the schema it was given
 */
TEST_F(OpNodeTest, SelectFileNode_GetSchema1) {
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectFileNode op (0, childSchema, selects["A"]);

	ASSERT_EQ(9, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(5, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(6, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(7, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * SelectFileNode:: typical usage
 */
TEST_F(OpNodeTest, SelectFileNode_GetSchema2) {
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectFileNode op (0, A, selects["A"]);

	ASSERT_EQ(3, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
}

/*
 * SelectPipeNode should not fail if the schema it was given was empty
 */
TEST_F(OpNodeTest, SelectFileNode_GetSchema3) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&emptySchema));
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	SelectFileNode op (0, emptySchema, selects["B"]);

	ASSERT_EQ(0, op.GetSchema()->GetNumAtts());
}


// ProjectNode
/**
 * ProjectNode::GetSchema should correctly project away undesired attributes
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema1) {
	vector<RelAttPair> attsToKeep;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(attsToKeep);

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(false));

	ASSERT_EQ(4, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(2, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(3, op.GetSchema()->Find("C.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
}

/**
 * ProjectNode::GetSchema should return all the attributes if no attributes are to be projected away
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema2) {
	vector<RelAttPair> attsToKeep;
	attsToKeep.push_back(RelAttPair("A","a"));
	attsToKeep.push_back(RelAttPair("A","b"));
	attsToKeep.push_back(RelAttPair("A","c"));
	attsToKeep.push_back(RelAttPair("B","a"));
	attsToKeep.push_back(RelAttPair("B","b"));
	attsToKeep.push_back(RelAttPair("C","c"));
	attsToKeep.push_back(RelAttPair("D","d"));
	attsToKeep.push_back(RelAttPair("E","e"));
	attsToKeep.push_back(RelAttPair("E","b"));

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(false));

	ASSERT_EQ(9, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(5, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(6, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(7, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * ProjectNode::GetSchema should handle the case where every attribute is asked to be projected away
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema3) {
	vector<RelAttPair> attsToKeep;

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(false));

	ASSERT_EQ(0, op.GetSchema()->GetNumAtts());
}

/**
 * ProjectNode::GetSchema should add the aggregate if the child is an aggregate
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema4) {
	vector<RelAttPair> attsToKeep;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(attsToKeep);

	Attribute *sum= new Attribute[1];
	sum[0].relation=""; sum[0].name="Aggregate"; sum[0].myType=Double;

	Schema agg ("", 1, sum);
	Schema aggSchema(agg, childSchema);

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&aggSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(true));

	ASSERT_EQ(5, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));
	EXPECT_EQ(1, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("C.c"));

	EXPECT_EQ(Double, op.GetSchema()->FindType("Aggregate"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
}

/**
 * ProjectNode::GetSchema should add the aggregate if the child is an aggregate and handle the case
 * where no attributes are projected away
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema5) {
	Attribute *sum = new Attribute[1];
	sum[0].relation=""; sum[0].name="Aggregate"; sum[0].myType=Int;
	Schema agg ("", 1, sum);
	Schema aggSchema(agg, childSchema);

	vector<RelAttPair> attsToKeep;
	attsToKeep.push_back(RelAttPair("A","a"));
	attsToKeep.push_back(RelAttPair("A","b"));
	attsToKeep.push_back(RelAttPair("A","c"));
	attsToKeep.push_back(RelAttPair("B","a"));
	attsToKeep.push_back(RelAttPair("B","b"));
	attsToKeep.push_back(RelAttPair("C","c"));
	attsToKeep.push_back(RelAttPair("D","d"));
	attsToKeep.push_back(RelAttPair("E","e"));
	attsToKeep.push_back(RelAttPair("E","b"));

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&aggSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(true));

	ASSERT_EQ(10, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(3, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(5, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(6, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(7, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(9, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("Aggregate"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * ProjectNode::GetSchema should add the aggregate if the child is an aggregate and handle the case
 * where every attribute is projected away (except the aggregate)
 */
TEST_F(OpNodeTest, ProjectNode_GetSchema6) {
	Attribute *sum = new Attribute[1];
	sum[0].relation=""; sum[0].name="Aggregate"; sum[0].myType=Double;
	Schema agg ("", 1, sum);
	Schema aggSchema(agg, childSchema);

	vector<RelAttPair> attsToKeep;

	PartialProjectNode op (0, &child, attsToKeep);

	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&aggSchema));
	EXPECT_CALL(op, ContainsAggregate()).
			WillRepeatedly(Return(true));

	ASSERT_EQ(1, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));

	EXPECT_EQ(Double, op.GetSchema()->FindType("Aggregate"));
}


// JoinNode
/**
 * JoinNode::GetSchema should correctly merge two schemas. This will assume that there is never a
 * case where one (or both) sides have an empty schema
 */
TEST_F(OpNodeTest, JoinNode_GetSchema1) {
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	MockOpNode left;
	MockOpNode right;

	EXPECT_CALL(left, GetSchema()).
			WillRepeatedly(Return(&A));
	EXPECT_CALL(right, GetSchema()).
				WillRepeatedly(Return(&B));

	JoinNode op (0, &left, 50, &right, 10, joins["A"]["B"]);

	ASSERT_EQ(5, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
}

/**
 * Join should correctly organize the children. If the number of tuples expected out of left is less
 * than right, then left should be the rightchild and vice versa.
 */
TEST_F(OpNodeTest, JoinNode_GetSchema2) {
	SelectMap selects;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetWhere(selects, joins);

	MockOpNode left;
	MockOpNode right;

	EXPECT_CALL(left, GetSchema()).
			WillRepeatedly(Return(&D));
	EXPECT_CALL(right, GetSchema()).
				WillRepeatedly(Return(&E));

	JoinNode op (0, &left, 10, &right, 50, joins["D"]["E"]);

	ASSERT_EQ(3, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(1, op.GetSchema()->Find("E.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("D.d"));


	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
}


// DuplicateRemoval
/**
 * DuplicateRemovalNode::GetSchema should return it's child's schema
 */
TEST_F(OpNodeTest, DuplicateRemovalNode_GetSchema1) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	vector<RelAttPair> duplicates;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(duplicates);

	DuplicateRemovalNode op (0, &child, duplicates);

	ASSERT_EQ(9, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(3, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(5, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(6, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(7, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * DuplicateRemovalNode::GetSchema typical usage
 */
TEST_F(OpNodeTest, DuplicateRemovalNode_GetSchema2) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&A));
	vector<RelAttPair> duplicates;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(duplicates);

	DuplicateRemovalNode op (0, &child, duplicates);

	ASSERT_EQ(3, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
}

/*
 * DuplicateRemovalNode::GetSchema should not fail to get the schema even if it was empty
 */
TEST_F(OpNodeTest, DuplicateRemovalNode_GetSchema3) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&emptySchema));
	vector<RelAttPair> duplicates;
	JoinMap joins;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(duplicates);

	DuplicateRemovalNode op (0, &child, duplicates);

	ASSERT_EQ(0, op.GetSchema()->GetNumAtts());
}


// SumNode
/**
 * SumNode shoud return a a schema with one attribute: Aggregate. The type will depend on the
 * function. In this case, it should be a double. The assumption is made that this cannot receive
 * an empty schema because you cannot aggregate over nothing
 */
TEST_F(OpNodeTest, SumNode_GetSchema1) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	vector<RelAttPair> duplicates;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetSelect(duplicates);

	SumNode op (0, &child, sql.Function());

	ASSERT_EQ(1, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));

	EXPECT_EQ(Double, op.GetSchema()->FindType("Aggregate"));
}

/**
 * SumNode shoud return a a schema with one attribute: Aggregate. The type will depend on the
 * function. In this case, it should be an int
 */
TEST_F(OpNodeTest, SumNode_GetSchema2) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&A));
	vector<RelAttPair> duplicates;

	string str = " SELECT SUM ( A.a ) ";
	str.append(" FROM RelA AS A ");
	str.append(" WHERE A.a > 0 ");

	SQL sql (stats);
	sql.Parse(str);
	sql.GetSelect(duplicates);

	SumNode op (0, &child, sql.Function());

	ASSERT_EQ(1, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("Aggregate"));
}


// GroupBy
/**
 * GroupBy::GetSchema should return its child's schema if it is not an aggregate
 */
TEST_F(OpNodeTest, GroupByNode_GetSchema1) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&A));
	vector<RelAttPair> group;
	SQL sql (stats);
	string str = " SELECT A.c, A.a  ";
	str.append(" FROM RelA AS A ");
	str.append(" WHERE A.a > 0 ");
	str.append(" GROUP BY A.c, A.a ");

	sql.Parse(str);
	sql.GetGroup(group);

	GroupByNode op (0, &child, group, sql.Function());

	ASSERT_EQ(3, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
}

/**
 * GroupBy::GetSchema should produce a function with the aggregate appended to the beginning of its
 * child's schema (if it an aggregate). The name of the attribute should be Aggregate. In this
 * case, it should be of type double
 */
TEST_F(OpNodeTest, GroupByNode_GetSchema2) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&childSchema));
	vector<RelAttPair> group;
	SQL sql (stats);
	sql.Parse(query);
	sql.GetGroup(group);

	GroupByNode op (0, &child, group, sql.Function());

	ASSERT_EQ(10, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(3, op.GetSchema()->Find("A.c"));
	EXPECT_EQ(4, op.GetSchema()->Find("B.a"));
	EXPECT_EQ(5, op.GetSchema()->Find("B.b"));
	EXPECT_EQ(6, op.GetSchema()->Find("C.c"));
	EXPECT_EQ(7, op.GetSchema()->Find("D.d"));
	EXPECT_EQ(8, op.GetSchema()->Find("E.e"));
	EXPECT_EQ(9, op.GetSchema()->Find("E.b"));

	EXPECT_EQ(Double, op.GetSchema()->FindType("Aggregate"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("B.a"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("B.b"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("C.c"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("D.d"));
	EXPECT_EQ(Double, op.GetSchema()->FindType("E.e"));
	EXPECT_EQ(String, op.GetSchema()->FindType("E.b"));
}

/**
 * GroupBy::GetSchema should produce a function with the aggregate appended to the beginning of its
 * child's schema (if it an aggregate). The name of the attribute should be Aggregate. In this
 * case, it should be of type int
 */
TEST_F(OpNodeTest, GroupByNode_GetSchema3) {
	EXPECT_CALL(child, GetSchema()).
			WillRepeatedly(Return(&A));
	vector<RelAttPair> group;
	SQL sql (stats);
	string str = " SELECT SUM(A.b),  A.c, A.a";
	str.append(" FROM RelA AS A ");
	str.append(" WHERE A.a > 0 ");
	str.append(" GROUP BY A.c, A.a ");

	sql.Parse(str);
	sql.GetGroup(group);

	GroupByNode op (0, &child, group, sql.Function());

	ASSERT_EQ(4, op.GetSchema()->GetNumAtts());
	EXPECT_EQ(0, op.GetSchema()->Find("Aggregate"));
	EXPECT_EQ(1, op.GetSchema()->Find("A.a"));
	EXPECT_EQ(2, op.GetSchema()->Find("A.b"));
	EXPECT_EQ(3, op.GetSchema()->Find("A.c"));

	EXPECT_EQ(Int, op.GetSchema()->FindType("Aggregate"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.a"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.b"));
	EXPECT_EQ(Int, op.GetSchema()->FindType("A.c"));
}
