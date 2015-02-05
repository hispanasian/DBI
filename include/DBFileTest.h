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

class DBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	DBFile file = DBFile(mockFile, rfile, config);
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;
	StrictMock<MockSchema> schema;

	char *path = "asdasdasd";
	char *header = "asdasdasd.header";

	off_t CursorIndex() { return file.cursorIndex; }

	void SetCursorIndex(off_t offset) { file.cursorIndex = offset; }

	off_t LastIndex() { return file.lastIndex; }

	void SetLastIndex(off_t offset) { file.lastIndex = offset; }

	File GetFile() { return file.file; }

	Page *GetCursor() { return file.cursor; }

	void SetCursor(Page &page) {
		delete file.cursor;
		file.cursor = &page;
	}

	void SetCursorNull() { file.cursor= NULL; }

	Page *GetLast() { return file.last; }

	void SetLast(Page &page) {
		delete file.last;
		file.last = &page;
	}

	void SetLastNull() { file.last = NULL; }

	void Load(Schema &myschema, char *loadpath, Record &record) { file.Load(myschema, loadpath, record); }
};

#endif
