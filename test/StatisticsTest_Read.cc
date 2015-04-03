#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * Read should populate the map with the contents from RawFile.
 */
TEST_F(StatisticsTest, Read1) {
	StrictMock<MockRawFile> file;
	InSequence seq;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Relations"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1=7"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2=80"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel3=2"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Attributes"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1.hey=10"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1.jude=50"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2.hey=5"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2.soul=4"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2.sister=3"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(Return(false));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));

	Read("file.txt", file);
	EXPECT_EQ(7, map["rel1"].numTuples);
	EXPECT_EQ(80, map["rel2"].numTuples);
	EXPECT_EQ(2, map["rel3"].numTuples);

	EXPECT_EQ(10, map["rel1"].atts["hey"]);
	EXPECT_EQ(50, map["rel1"].atts["jude"]);
	EXPECT_EQ(5, map["rel2"].atts["hey"]);
	EXPECT_EQ(4, map["rel2"].atts["soul"]);
	EXPECT_EQ(3, map["rel2"].atts["sister"]);
}

/**
 * Read should not fail if Relations and Attributes is empty
 */
TEST_F(StatisticsTest, Read2) {
	StrictMock<MockRawFile> file;
	InSequence seq;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Relations"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Attributes"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(Return(false));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));
	Read("file.txt", file);
}

/**
 * Read should not fail Attributes is empty
 */
TEST_F(StatisticsTest, Read3) {
	StrictMock<MockRawFile> file;
	InSequence seq;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Relations"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1=7"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2=80"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel3=2"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Attributes"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(Return(false));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));

	Read("file.txt", file);
	EXPECT_EQ(7, map["rel1"].numTuples);
	EXPECT_EQ(80, map["rel2"].numTuples);
	EXPECT_EQ(2, map["rel3"].numTuples);
}

/**
 * Read should throw a out_of_range exception if the file contains a relation.attribute pair for
 * which the relation was not declared
 */
/**
 * Read should populate the map with the contents from RawFile.
 */
TEST_F(StatisticsTest, Read4) {
	StrictMock<MockRawFile> file;
	InSequence seq;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Relations"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1=7"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Attributes"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1.hey=10"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel1.jude=50"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("rel2.hey=5"), Return(true)));

	ASSERT_THROW(Read("file.txt", file), std::out_of_range);
}
