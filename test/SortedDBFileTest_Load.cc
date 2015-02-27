#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/RawFile.h"
#include "../include/Comparison.h"
#include "../include/DBConfig.h"

#include "../include/SortedDBFileTest.h"

/*
	Load should add a single record to the file
*/
TEST_F(SortedDBFileTest, Load1) {
	StrictMock<MockRecord> record;
	StrictMock<MockPipe> in;
	string loadpath = "loadpath";
	char *path = (char *)loadpath.c_str();
	RawFile temp;
	temp.Open(loadpath);
	temp.Append("junk");
	temp.Close();
	
	SetIn(&in);

	{
		InSequence seq;
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(1));
		EXPECT_CALL(in, Insert(&record));	
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(0));
	}

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));

	Load(schema, path, record);

	EXPECT_EQ(Writing, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	remove(path);
}

/*
	Load should add a multiple records to the file
*/
TEST_F(SortedDBFileTest, Load2) {
	StrictMock<MockRecord> record;
	StrictMock<MockPipe> in;
	string loadpath = "loadpath";
	char *path = (char *)loadpath.c_str();
	RawFile temp;
	temp.Open(loadpath);
	temp.Append("junk");
	temp.Close();
	
	SetIn(&in);

	{
		InSequence seq;
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(1));
		EXPECT_CALL(in, Insert(&record));
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(1));
		EXPECT_CALL(in, Insert(&record));
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(1));
		EXPECT_CALL(in, Insert(&record));	
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
			WillOnce(Return(0));
	}

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));

	Load(schema, path, record);

	EXPECT_EQ(Writing, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	remove(path);
}

/**
 * Load should throw a runtime_error if the file does not exist.
 */
TEST_F(SortedDBFileTest, Load3) {
	StrictMock<MockRecord> record;
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(false));
	EXPECT_THROW(Load(schema, path, record), std::runtime_error);
}

/*
	Load should silently return if there are no records in the file
*/
TEST_F(SortedDBFileTest, Load4) {
	StrictMock<MockRecord> record;
	StrictMock<MockPipe> in;
	string loadpath = "loadpath";
	char *path = (char *)loadpath.c_str();
	RawFile temp;
	temp.Open(loadpath);
	temp.Append("junk");
	temp.Close();
	
	SetIn(&in);

	EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
		WillOnce(Return(0));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));

	Load(schema, path, record);

	EXPECT_EQ(Writing, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	remove(path);
}