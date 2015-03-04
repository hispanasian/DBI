#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/RawFile.h"
#include "../include/Comparison.h"
#include "../include/DBConfig.h"

#include "../include/SortedDBFileTest.h"

/**
 * SortedDBFile::Add should, if in Reading mode, get set to Writing mode and add the record to in.
 * It should also set GetNextState to NoCNF regardless of any other state.
 */
TEST_F(SortedDBFileTest, Add1) {
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFile> temp;
	StrictMock<MockBigQ> q;
	StrictMock<Record> rec;

	SetIn(&in);
	SetOut(&out);
	SetRWState(Reading);
	SetGetNextState(NoCNF);

	// Check to make sure the real work gets called.
	EXPECT_CALL(in, Insert(&rec));

	sorteddb->Add(rec);
	EXPECT_EQ(Writing, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetIn(NULL);
	SetOut(NULL);
	delete mock;
}

/**
 * SortedDBFile::Add should, if in Writing mode, get set to Writing mode and add the record to in.
 * It should also set GetNextState to NoCNF regardless of any other state.
 */
TEST_F(SortedDBFileTest, Add2) {
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFile> temp;
	StrictMock<MockBigQ> q;
	StrictMock<Record> rec;

	SetIn(&in);
	SetOut(&out);
	SetRWState(Writing);
	SetGetNextState(UseCNF);

	// Check to make sure the real work gets called.
	EXPECT_CALL(in, Insert(&rec));

	sorteddb->Add(rec);
	EXPECT_EQ(Writing, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetIn(NULL);
	SetOut(NULL);
	delete mock;
}
