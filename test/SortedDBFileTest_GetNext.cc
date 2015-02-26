#include "SortedDBFileTest.h"
#include "SortedDBFile.h"

/**
 * SortedDBFile::GetNext should return 1 if a Record is found.
 */
TEST_F(SortedDBFileTest, GetNext1) {
	StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;

	//SetRWState(Reading);
	SetGetNextState(NoCNF);

	InSequence seq;
	EXPECT_CALL(*mock, Flush());
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			WillOnce(Return(1));

	EXPECT_EQ(1, sorteddb->GetNext(rec));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::GetNext should return 0 if no Record is found.
 */
TEST_F(SortedDBFileTest, GetNext2) {
	StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;

	//SetRWState(Writing);
	SetGetNextState(UseCNF);

	InSequence seq;
	EXPECT_CALL(*mock, Flush());
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			WillOnce(Return(0));

	EXPECT_EQ(0, sorteddb->GetNext(rec));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(UseCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}
