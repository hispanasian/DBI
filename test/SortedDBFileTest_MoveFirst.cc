#include "SortedDBFileTest.h"
#include "SortedDBFile.h"

/*
	If we're in reading mode and the file is not empty,
	read the first page into the cursor
*/
TEST_F(SortedDBFileTest, MoveFirst1) {
	MockPage cursor;
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);

	SetRWState(Reading);

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));

	// Arbitrary Calls
	EXPECT_CALL(*mock, Flush()).
			Times(AtMost(1));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(10));

	sorteddb->MoveFirst();
	EXPECT_EQ(0, GetCursorIndex());
	EXPECT_EQ(Reading, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

/*
	If we're in writing mode and the file is not empty,
	flush the pipe, read the first page into memory, and 
	set the cursor index to 0
*/
TEST_F(SortedDBFileTest, MoveFirst2) {
	MockPage cursor;
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(10);

	SetRWState(Writing);

	Sequence s1;
	EXPECT_CALL(*mock, Flush()).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetPage(&cursor, 0)).
			InSequence(s1);

	// Arbitrary Calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(11));

	sorteddb->MoveFirst();
	EXPECT_EQ(0, GetCursorIndex());
	EXPECT_EQ(Reading, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}


/*
	If we're in reading mode and the file is empty,
	set the cursor index to 0
*/
TEST_F(SortedDBFileTest, MoveFirst3) {
	MockPage cursor;
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(8);

	SetRWState(Reading);

	// Arbitrary Calls
	EXPECT_CALL(*mock, Flush()).
			Times(AtMost(1));
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	sorteddb->MoveFirst();
	EXPECT_EQ(0, GetCursorIndex());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

/*
	If we're in writing mode and the file is empty,
	flush the pipe and set the cursor index to 0
*/
TEST_F(SortedDBFileTest, MoveFirst4) {
	MockPage cursor;
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(10);

	SetRWState(Writing);

	EXPECT_CALL(*mock, Flush());

	// Arbitrary Calls
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	sorteddb->MoveFirst();
	EXPECT_EQ(0, GetCursorIndex());
	EXPECT_EQ(Reading, GetRWState());
	EXPECT_EQ(NoCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}
