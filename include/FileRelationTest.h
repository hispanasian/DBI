/*
 * FileRelationTest.h
 *
 *  Created on: Mar 19, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_FILERELATIONTEST_H_
#define INCLUDE_FILERELATIONTEST_H_

#include "FileRelation.h"

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


class FileRelationTest: public ::testing::Test {
public:
	StrictMock<MockRawFile> rfile;
	StrictMock<MockFile> file;
	StrictMock<MockDBConfig> config;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockHeapDBFile> relation;
	FileRelation rel = FileRelation(rfile, file, config, comp, relation);

	bool Add(Pipe &in, Record &rec) { return rel.Add(in, rec); }
};

#endif /* INCLUDE_FILERELATIONTEST_H_ */
