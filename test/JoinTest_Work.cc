#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/JoinTest.h"

/**
 * If cnf.GetSortOrders fails, call BlockNestedLoopJoin.
 */
TEST_F(JoinTest, Work1) {
	InSequence seq;
	EXPECT_CALL(cnf, GetSortOrders(Ref(orderL), Ref(orderR))).
			WillOnce(Return(0));
	EXPECT_CALL(op, BlockNestedLoopJoin(Ref(inL), Ref(inR), Ref(out), Ref(cnf), Ref(literal)));

	Work(inL, inR, out, cnf, literal, comp, orderL, orderR);
}

/**
 * If cnf.GetSortOrders passes, call SortMergeJoin
 */
TEST_F(JoinTest, Work2) {
	InSequence seq;
	EXPECT_CALL(cnf, GetSortOrders(Ref(orderL), Ref(orderR))).
			WillOnce(Return(1));
	EXPECT_CALL(op, SortMergeJoin(Ref(inL), Ref(inR), Ref(out), Ref(orderL), Ref(orderR)));
	Work(inL, inR, out, cnf, literal, comp, orderL, orderR);
}
