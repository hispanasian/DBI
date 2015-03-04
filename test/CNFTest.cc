#include "../include/CNFTest.h"
#include <string>

/*
 * CNFTest.cc
 *
 *  Created on: Feb 25, 2015
 *      Author: cvasquez
 */

/**
 * CNF::IsSortableAttribute should return true only if:
 * 1. attr is the only attribute in its disjunction
 * 2. The attribute is being compared to a literal
 * 3. The comparison is an Equals comparison.
 */
TEST_F(CNFTest, IsSortableAttribute1) {
	SetNumAnds(1);
	SetOrLens(0, 1);
	SetComparison(GetOrList(0,0), Left, 0, Literal, 5, Int, Equals);

	EXPECT_EQ(true, cnf.IsSortableAttribute(0));
}

/**
 * CNF::IsSortableAttribute should return false if attr is not the only attribute in a disjunction
 */
TEST_F(CNFTest, IsSortableAttribute2) {
	SetNumAnds(3);
	SetOrLens(0,1);
	SetOrLens(1,2);
	SetOrLens(2,1);
	SetComparison(GetOrList(0,0), Left, 0, Literal, 5, Int, Equals);
	SetComparison(GetOrList(1,0), Literal, 0, Literal, 1, Int, Equals);
	SetComparison(GetOrList(1,1), Left, 2, Literal, 5, Int, Equals);
	SetComparison(GetOrList(2,0), Left, 3, Literal, 5, Int, Equals);

	EXPECT_EQ(false, cnf.IsSortableAttribute(1));
}

/**
 * CNF::IsSortableAttribute should return true if attr is found in multiple disjunctions (even
 * though this is dumb)
 */
TEST_F(CNFTest, IsSortableAttribute3) {
	SetNumAnds(3);
	SetOrLens(0,1);
	SetOrLens(1,2);
	SetOrLens(2,1);
	SetComparison(GetOrList(0,0), Left, 0, Literal, 0, Int, Equals);
	SetComparison(GetOrList(1,0), Literal, 0, Literal, 0, Int, Equals);
	SetComparison(GetOrList(1,1), Left, 2, Literal, 0, Int, Equals);
	SetComparison(GetOrList(2,0), Left, 0, Literal, 10, Int, Equals);

	EXPECT_EQ(true, cnf.IsSortableAttribute(0));
}

/**
 * CNF::IsSortableAttribute should return false if attr is not compared to with Equals
 */
TEST_F(CNFTest, IsSortableAttribute4) {
	SetNumAnds(3);
	SetOrLens(0,1);
	SetOrLens(1,2);
	SetOrLens(2,1);
	SetComparison(GetOrList(0,0), Left, 3, Literal, 5, Int, GreaterThan);
	SetComparison(GetOrList(1,0), Literal, 0, Literal, 3, Int, GreaterThan);
	SetComparison(GetOrList(1,1), Left, 3, Literal, 5, Int, GreaterThan);
	SetComparison(GetOrList(2,0), Left, 3, Literal, 5, Int, GreaterThan);

	EXPECT_EQ(false, cnf.IsSortableAttribute(3));
}

/**
 * CNF::IsSortableAttribute should return true if attr is alone in a disjunction and compared with
 * Equals.
 */
TEST_F(CNFTest, IsSortableAttribute5) {
	SetNumAnds(3);
	SetOrLens(0,1);
	SetOrLens(1,2);
	SetOrLens(2,1);
	SetComparison(GetOrList(0,0), Left, 3, Literal, 5, Int, GreaterThan);
	SetComparison(GetOrList(1,0), Literal, 0, Literal, 1, Int, GreaterThan);
	SetComparison(GetOrList(1,1), Left, 2, Literal, 5, Int, GreaterThan);
	SetComparison(GetOrList(2,0), Left, 3, Literal, 5, Int, Equals);

	EXPECT_EQ(true, cnf.IsSortableAttribute(3));
}

/**
 * CNF::IsSortableAttribute should return false if there are only literals in a valid disjunction
 */
TEST_F(CNFTest, IsSortableAttribute6) {
	SetNumAnds(3);
	SetOrLens(0,1);
	SetOrLens(1,2);
	SetOrLens(2,1);
	SetComparison(GetOrList(0,0), Literal, 3, Literal, 5, Int, Equals);
	SetComparison(GetOrList(1,0), Literal, 0, Literal, 1, Int, GreaterThan);
	SetComparison(GetOrList(1,1), Literal, 2, Literal, 5, Int, GreaterThan);
	SetComparison(GetOrList(2,0), Left, 4, Literal, 5, Int, Equals);

	EXPECT_EQ(false, cnf.IsSortableAttribute(3));
}

/**
 * CNF::IsSortableAttribute should return false if cnf is empty
 */
TEST_F(CNFTest, IsSortableAttribute7) {
	SetNumAnds(0);

	EXPECT_EQ(false, cnf.IsSortableAttribute(3));
}

/**
 * CNF::MakeQuery should return true when a sortOrder contains a single attribute that exists as
 * a single Equals disjunction in CNF
 */
TEST_F(CNFTest, MakeQuery1) {
	OrderMaker sortOrder = OrderMaker("0 int");
	OrderMaker query;

	SetNumAnds(1);
	SetOrLens(0, 1);
	SetComparison(GetOrList(0,0), Left, 0, Literal, 5, Int, Equals);

	EXPECT_EQ(true, cnf.MakeQuery(sortOrder, query));
	EXPECT_EQ(0, strcmp("0 int", query.ToString().c_str()));
}

/**
 * CNF::MakeQuery should return false when sortOrder is empty
 */
TEST_F(CNFTest, MakeQuery2) {
	OrderMaker sortOrder = OrderMaker("");
	OrderMaker query;

	SetNumAnds(1);
	SetOrLens(0, 1);
	SetComparison(GetOrList(0,0), Left, 0, Literal, 5, Int, Equals);

	EXPECT_EQ(false, cnf.MakeQuery(sortOrder, query));
	EXPECT_EQ(0, strcmp("", query.ToString().c_str()));
}

/**
 * CNF::MakeQuery should return false when cnf is empty
 */
TEST_F(CNFTest, MakeQuery3) {
	OrderMaker sortOrder = OrderMaker("");
	OrderMaker query;

	SetNumAnds(0);

	EXPECT_EQ(false, cnf.MakeQuery(sortOrder, query));
	EXPECT_EQ(0, strcmp("", query.ToString().c_str()));
}

/**
 * CNF::MakeQuery should return true when given a sort on attributes 2, 1, 4, and 5 and a CNF with
 * attributes 2, 1 and 5. Furthermore, query should give an order on 2 and 1 (in that order)
 */
TEST_F(CNFTest, MakeQuery4) {
	OrderMaker sortOrder = OrderMaker("2 int 1 double 4 string 5 int");
	OrderMaker query;

	SetNumAnds(3);
	SetOrLens(0, 1);
	SetOrLens(1, 1);
	SetOrLens(2, 1);
	SetComparison(GetOrList(0,0), Left, 1, Literal, 5, Double, Equals);
	SetComparison(GetOrList(1,0), Literal, 0, Right, 2, Int, Equals);
	SetComparison(GetOrList(2,0), Left, 5, Literal, 5, Int, Equals);

	EXPECT_EQ(true, cnf.MakeQuery(sortOrder, query));
	EXPECT_EQ(0, strcmp("0 int 1 double", query.ToString().c_str()));
}

/**
 * CNF::MakeQuery should return false when sortOrder has order 2, 1, 4 and 5 and cnf only has
 * attributes 1, 4 and 5
 */
TEST_F(CNFTest, MakeQuery5) {
	OrderMaker sortOrder = OrderMaker("2 int 1 double 4 string 5 int");
	OrderMaker query;

	SetNumAnds(3);
	SetOrLens(0, 1);
	SetOrLens(1, 1);
	SetOrLens(2, 1);
	SetComparison(GetOrList(0,0), Left, 1, Literal, 5, Double, Equals);
	SetComparison(GetOrList(1,0), Literal, 0, Right, 4, Int, Equals);
	SetComparison(GetOrList(2,0), Left, 5, Literal, 5, Int, Equals);

	EXPECT_EQ(false, cnf.MakeQuery(sortOrder, query));
	EXPECT_EQ(0, strcmp("", query.ToString().c_str()));
}
