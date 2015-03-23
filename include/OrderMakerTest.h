/*
 * OrderMakerTest.h
 * Fixtured for OrderMaker
 *  Created on: Feb 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_ORDERMAKERTEST_H_
#define INCLUDE_ORDERMAKERTEST_H_

#include "Comparison.h"

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
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

class OrderMakerTest: public ::testing::Test {
public:
	OrderMaker *order;

	int GetNumAtts() { return order->numAtts; }
	void SetNumAtts(int i) { order->numAtts = i; }
	int GetWhichAtt(int i) { return order->whichAtts[i]; }
	void SetWhichAtt(int i, int j) { order->whichAtts[i] = j; }
	Type GetWhichType(int i) { return order->whichTypes[i]; }
	void SetWhichType(int i, Type j) { order->whichTypes[i] = j; }
};



#endif /* INCLUDE_ORDERMAKERTEST_H_ */
