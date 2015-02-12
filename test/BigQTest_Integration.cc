#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "BigQ.h"
#include "Pipe.h"
#include "Comparison.h"

TEST(BigQTest, Integration1) {
	Pipe in;
	Pipe out;
	OrderMaker order;
	int runlen = 10;
	BigQ bigq(in, out, order, runlen);
}