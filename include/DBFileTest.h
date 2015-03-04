#ifndef DBFILETEST_H
#define DBFILETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/DBFile.h"
#include "../include/File.h"
#include "../include/Schema.h"
#include "GenericDBFile.h"

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
using ::testing::Pointee;
using ::testing::NotNull;

class DBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	StrictMock<MockComparisonEngine> comp;
	DBFile file = DBFile(mockFile, rfile, config, comp);
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;
	StrictMock<MockSchema> schema;

	char *path = "asdasdasd";
	char *header = "asdasdasd.header";

	void Load(Schema &myschema, char *loadpath, Record &record) { file.Load(myschema, loadpath, record); }

	void SetDB(GenericDBFile *db) {
		delete file.delegate;
		file.delegate = db;
	}

	GenericDBFile* GetDB() { return file.delegate; }
};

#endif
