#ifndef INCLUDE_TPMMSTEST_H_
#define INCLUDE_TPMMSTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "MockClasses.h"
#include "BigQ.h"

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

class TPMMSTest: public ::testing::Test {
public:
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFile> file;
	StrictMock<MockPage> page;
	StrictMock<MockRecord> *rec = new StrictMock<MockRecord>;
	StrictMock<MockComparisonEngine> comp;
	OrderMaker order;
	vector<off_t> runPos;
	vector<Record *> run;
	int runlen = 3;

	TPMMS tpmms = TPMMS(in, out, file, page, comp, order, runPos, run, runlen);

	void SortRun() { tpmms.SortRun(); }
	void RunToFile(off_t &totalPageCount) { tpmms.RunToFile(totalPageCount); }
	bool AddRecord(Record* rec) { return tpmms.AddRecord(rec); }
	void Phase1() { tpmms.Phase1(); }
	void Phase2() { tpmms.Phase2(); }
	void Sort() { tpmms.Sort(); }
	int GetRunSizeInBytes() { return tpmms.runSizeInBytes; }
	void SetRunSizeInBytes(int size) { tpmms.runSizeInBytes = size; }
	int GetCurrRunSizeInBytes() { return tpmms.currRunSizeInBytes; }
	void SetCurrRunSizeInBytes(int size) { tpmms.currRunSizeInBytes = size; }
	void GetNextRecord(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft) { tpmms.GetNextRecord(min, heads, runIndex, pages, runsLeft); }
	int FindMin(int size, Record **&heads) { return tpmms.FindMin(size, heads); }
	vector<Record *> GetRun() { return run; }
};

#endif /* INCLUDE_TPMMSTEST_H_ */
