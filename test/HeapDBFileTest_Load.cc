#include "../include/HeapDBFileTest.h"
#include "../include/Schema.h"
#include "../include/RawFile.h"
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <string>

/**
 * DBFile::Load should call SuckNextRecord from record, give it an existing FILE, and add it to
 * last. This should happen until SuckNextRecord returns 0. If the FILE opens corretly and has 5
 * records, the call to SuckNextRecord and last.AddFile should occur 5 times.
 */
TEST_F(HeapDBFileTest, Load1) {
	StrictMock<MockRecord> record;
	SetDBToheapdb();
	SetLast(last);
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	RawFile temp;
	temp.Open(fpath);
	temp.Append("junk");
	temp.Close();
	{
		InSequence seq;
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				Times(4).
				WillRepeatedly(Return(1));
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				WillOnce(Return(0));
	}
	EXPECT_CALL(last, Append(&record)).
			Times(4).
			WillRepeatedly(Return(1));
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));
	this->Load(schema, path, record);
	remove(path);
	SetLastNull();
}

/**
 * DBFile::Load should throw a runtime_error if the file does not exist.
 */
TEST_F(HeapDBFileTest, Load2) {
	SetLast(last);
	SetDBToheapdb();
	StrictMock<MockRecord> record;
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(false));
	EXPECT_THROW(this->Load(schema, path, record), std::runtime_error);
	SetLastNull();
}

/**
 * DBFile::Load should not fail if only 1 record exists in the file.
 */
TEST_F(HeapDBFileTest, Load3) {
	StrictMock<MockRecord> record;
	SetDBToheapdb();
	SetLast(last);
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	RawFile temp;
	temp.Open(fpath);
	temp.Append("junk");
	temp.Close();
	{
		InSequence seq;
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				WillOnce(Return(1));
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				WillOnce(Return(0));
	}
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));
	this->Load(schema, path, record);
	remove(path);
	SetLastNull();
}

/**
 * DBFile::Load should not fail if the file has no records.
 */
TEST_F(HeapDBFileTest, Load4) {
	StrictMock<MockRecord> record;
	SetDBToheapdb();
	SetLast(last);
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	RawFile temp;
	temp.Open(fpath);
	temp.Append("junk");
	temp.Close();

	EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(0));
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));

	this->Load(schema, path, record);
	remove(path);
	SetLastNull();
}
