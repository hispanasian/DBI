#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MockClasses.h"
#include "../include/FileRelationTest.h"

/**
 * Add(*Record) should add the Record to the relation
 */
TEST_F(FileRelationTest, Add1) {
	StrictMock<MockRecord> rec;
	EXPECT_CALL(relation, Add(Ref(rec)));

	EXPECT_EQ(true, rel.Add(&rec));
}

/**
 * Add(*Pipe) should add all the records from Pipe to the relation
 */
TEST_F(FileRelationTest, Add2) {
	StrictMock<MockPipe> in;
	StrictMock<MockRecord> rec;

	Sequence s1;
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(true));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(in, Remove(NotNull())).
			InSequence(s1).
			WillOnce(Return(false));

	EXPECT_EQ(true, Add(in, rec));
}

/**
 * Add(*Pipe) should not fail if in is empty
 */
TEST_F(FileRelationTest, Add3) {
	StrictMock<MockPipe> in;
	StrictMock<MockRecord> rec;

	EXPECT_CALL(in, Remove(NotNull())).
			WillOnce(Return(false));

	EXPECT_EQ(true, Add(in, rec));
}

/**
 * Add(*Relation) should put every record from relation into this Relation. This method reset the
 * provided relation to the start (ie, it will call Reset on that relation)
 */
TEST_F(FileRelationTest, Add4) {
	StrictMock<MockRelation> temp;
	StrictMock<MockRecord> rec;

	Sequence s1;
	EXPECT_CALL(temp, Reset()).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&rec), Return(true)));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&rec), Return(true)));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&rec), Return(true)));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(DoAll(SetArgReferee<0>(&rec), Return(true)));
	EXPECT_CALL(relation, Add(Ref(rec))).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(Return(false));

	EXPECT_EQ(true, rel.Add(temp));
}

/**
 * Add(*Relation) should not fail if the provided relation is empty
 */
TEST_F(FileRelationTest, Add5) {
	StrictMock<MockRelation> temp;
	StrictMock<MockRecord> rec;

	Sequence s1;
	EXPECT_CALL(temp, Reset()).
			InSequence(s1);
	EXPECT_CALL(temp, GetNext(_)).
			InSequence(s1).
			WillOnce(Return(false));

	EXPECT_EQ(true, rel.Add(temp));

}
