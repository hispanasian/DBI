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

/**
 * SortedDBFile::GetNext with CNF should, if getNextState is NoCNF, call BinarySearch and then do
 * a linear scan looking for a record that conforms to the CNF.
 */
 TEST_F(SortedDBFileTest, GetNextCNF1) {
	StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	CNF cnf;

	//SetRWState(Writing);
	SetGetNextState(NoCNF);

	Sequence s1;
	EXPECT_CALL(*mock, Flush()).
			InSequence(s1);
	EXPECT_CALL(*mock, BinarySearch(Ref(lit), _)).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(comp, Compare(&rec, &lit, &cnf)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &cnf)).
			InSequence(s1).
			WillOnce(Return(1));

	// Order doesn't matter much
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			WillRepeatedly(Return(1));

	EXPECT_EQ(1, sorteddb->GetNext(rec, cnf, lit));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(UseCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

/**
 * SortedDBFile::GetNext with CNF should, if getNextState is UseCNF, don't call BinarySearch.
 */
TEST_F(SortedDBFileTest, GetNextCNF2) {
	StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	CNF cnf;

	//SetRWState(Writing);
	SetGetNextState(UseCNF);

	Sequence s1;
	EXPECT_CALL(*mock, Flush()).
			InSequence(s1);
	EXPECT_CALL(comp, Compare(&rec, &lit, &cnf)).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(&rec, &lit, &cnf)).
			InSequence(s1).
			WillOnce(Return(1));

	// Order doesn't matter much
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			WillRepeatedly(Return(1));

	EXPECT_EQ(1, sorteddb->GetNext(rec, cnf, lit));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(UseCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

 /**
 * SortedDBFile::GetNext with CNF should, if no Record can be found conforming to the CNF, return 0.
 */
/**
 * SortedDBFile::GetNext with CNF should, if getNextState is NoCNF, call BinarySearch and then do
 * a linear scan looking for a record that conforms to the CNF.
 */
 TEST_F(SortedDBFileTest, GetNextCNF3) {
	StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	CNF cnf;

	//SetRWState(Writing);
	SetGetNextState(NoCNF);

	Sequence s1;
	EXPECT_CALL(*mock, Flush()).
			InSequence(s1);
	EXPECT_CALL(*mock, BinarySearch(Ref(lit), _)).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(*scanner, GetNext(Ref(rec))).
			InSequence(s1).
			WillOnce(Return(0));

	// Order doesn't matter much
	EXPECT_CALL(comp, Compare(&rec, &lit, &cnf)).
			WillRepeatedly(Return(0));

	EXPECT_EQ(1, sorteddb->GetNext(rec, cnf, lit));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(UseCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
}

 /**
 * SortedDBFile::GetNext with CNF should, if getNextState is NoCNF and the BinarySearch fails,
 * return 0.
 */
 TEST_F(SortedDBFileTest, GetNextCNF4) {
	 StrictMock<MockPage> cursor;
	MockLinearScanner *scanner = new MockLinearScanner();
	MakeFlushlessSorted(sortInfo);
	SetCursor(&cursor);
	SetCursorIndex(5);
	SetLinearScanner(scanner);
	StrictMock<MockRecord> rec;
	StrictMock<MockRecord> lit;
	CNF cnf;

	//SetRWState(Writing);
	SetGetNextState(NoCNF);

	Sequence s1;
	EXPECT_CALL(*mock, Flush()).
			InSequence(s1);
	EXPECT_CALL(*mock, BinarySearch(Ref(lit), _)).
			InSequence(s1).
			WillOnce(Return(false));

	EXPECT_EQ(1, sorteddb->GetNext(rec, cnf, lit));
	//EXPECT_EQ(Reading, GetRWState()); // RW states are dealt with in Flush
	EXPECT_EQ(UseCNF, GetGetNextState());

	SetCursor(new Page());
	delete mock;
 }
