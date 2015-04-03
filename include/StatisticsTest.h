/*
 * StatisticsTest.h
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_STATISTICSTEST_H_
#define INCLUDE_STATISTICSTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Statistics.h"
#include "MockClasses.h"

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

class StatisticsTest: public::testing::Test {
public:
	std::unordered_map<std::string, StatData> map;
	std::unordered_map<std::string, std::string> lookup;
	Statistics stat = Statistics(map, lookup);

	void Read(char *name, RawFile &file) { stat.Read(name, file); }

	void Write(char *name, RawFile &file) { stat.Write(name, file); }

	void UpdateRel(char *relName, int numTuples) { stat.UpdateRel(relName, numTuples); }
};

#endif /* INCLUDE_STATISTICSTEST_H_ */