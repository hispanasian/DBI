#ifndef sorteddbDBFILETEST_H
#define sorteddbDBFILETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/DBFile.h"
#include "../include/File.h"
#include "../include/Schema.h"
#include "SortedDBFile.h"

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

class SortedDBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;
	StrictMock<MockSchema> schema;

	DBFile file = DBFile(mockFile, rfile, config, comp);
	SortedDBFile *sorteddb = new SortedDBFile(mockFile, rfile, config, comp, NULL);

	char *path = "asdasdasd";
	char *header = "asdasdasd.header";

	void MakeSortedDBFile(SortInfo *sortInfo) {
		delete sorteddb;
		sorteddb = new SortedDBFile(mockFile, rfile, config, comp, sortInfo);
	}

	File GetFile() { return file.file; }

	void Load(Schema &myschema, char *loadpath, Record &record) { file.Load(myschema, loadpath, record); }

	void SetDBTosorteddb() {
		file.delegate = sorteddb;
	}

	GenericDBFile* Delegate() { return file.delegate; }

	void SetDBNull() { file.delegate = NULL; }

	void SetSortInfo(SortInfo *sortInfo) { ((SortedDBFile*)file.delegate)-> sortInfo = sortInfo; }

	OrderMaker* GetOrder() { return ((SortedDBFile*)file.delegate)-> sortInfo -> myOrder; }

	int GetRunLength() { return ((SortedDBFile*)file.delegate)-> sortInfo -> runLength; }
};

#endif
