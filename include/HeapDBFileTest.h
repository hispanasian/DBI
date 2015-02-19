#ifndef heapdbDBFILETEST_H
#define heapdbDBFILETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/DBFile.h"
#include "../include/File.h"
#include "../include/Schema.h"
#include "HeapDBFile.h"

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

class HeapDBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;
	StrictMock<MockSchema> schema;

	DBFile file = DBFile(mockFile, rfile, config, comp);
	HeapDBFile *heapdb = new HeapDBFile(mockFile, rfile, config, comp);

	char *path = "asdasdasd";
	char *header = "asdasdasd.header";

	off_t CursorIndex() { return heapdb->cursorIndex; }

	void SetCursorIndex(off_t offset) { heapdb->cursorIndex = offset; }

	off_t LastIndex() { return heapdb->lastIndex; }

	void SetLastIndex(off_t offset) { heapdb->lastIndex = offset; }

	File GetFile() { return file.file; }

	Page *GetCursor() { return heapdb->cursor; }

	void SetCursor(Page &page) {
		delete heapdb->cursor;
		heapdb->cursor = &page;
	}

	void SetCursorNull() { heapdb->cursor= NULL; }

	Page *GetLast() { return heapdb->last; }

	void SetLast(Page &page) {
		delete heapdb->last;
		heapdb->last = &page;
	}

	void SetLastNull() { heapdb->last = NULL; }

	void Load(Schema &myschema, char *loadpath, Record &record) { file.Load(myschema, loadpath, record); }

	void SetDBToheapdb() {
		file.delegate = heapdb;
	}

	void SetDBNull() { file.delegate = NULL; }
};

#endif
