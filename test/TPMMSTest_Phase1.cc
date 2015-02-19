#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/BigQ.h"
#include "../include/TPMMSTest.h"

/**
 * Phase1 should return 0 (and do almost nothing) if in has nothing
 */
TEST_F(TPMMSTest, Phase1Test1) {
	SetRec(rec);
	EXPECT_CALL(in, Remove(_)).
			WillOnce(Return(0));

	Phase1();
	EXPECT_EQ(0, run.size());
}
