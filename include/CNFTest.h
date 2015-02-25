/*
 * CNFTest.h
 *
 *  Created on: Feb 25, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_CNFTEST_H_
#define INCLUDE_CNFTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "Comparison.h"
#include "Defs.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

class CNFTest: public ::testing::Test {
public:
	CNF cnf;

	Comparison &GetOrList(int x, int y) { return cnf.orList[x][y]; }

	void SetOrLens(int x, int lens) { cnf.orLens[x] = lens; }

	int GetOrLens(int x) { return cnf.orLens[x]; }

	void SetNumAnds(int num) { cnf.numAnds = num; }

	int GetNumAnds() { return cnf.numAnds; }

	void SetComparison(Comparison &comp, Target op1, int att1, Target op2, int att2, Type atttype, CompOperator op) {
		comp.operand1 = op1;
		comp.operand2 = op2;
		comp.whichAtt1 = att1;
		comp.whichAtt2 = att2;
		comp.attType = atttype;
		comp.op = op;
	}
};


#endif /* INCLUDE_CNFTEST_H_ */
