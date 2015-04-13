#include "StatisticsTest.h"

/*
 * Work will sum up all the integers
 */

/**
 * Write should write the map to a file
 */
TEST_F(StatisticsTest, Write1) {
	StrictMock<MockRawFile> file;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, Append("Relations\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel1=65.000000\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel2=12.300000\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel3=97.000000\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("Attributes\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel1.att1=1\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel1.att2=2\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel1.att3=3\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("rel3.att4=4\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));

	map["rel1"].numTuples = 65;
	map["rel2"].numTuples = 12.3;
	map["rel3"].numTuples = 97;

	map["rel1"].atts["att1"] = 1;
	map["rel1"].atts["att2"] = 2;
	map["rel1"].atts["att3"] = 3;
	map["rel3"].atts["att4"]= 4;

	Write("file.txt", file);
	EXPECT_EQ(65, map["rel1"].numTuples);
	EXPECT_EQ(12.3, map["rel2"].numTuples);
	EXPECT_EQ(97, map["rel3"].numTuples);

	EXPECT_EQ(1, map["rel1"].atts["att1"]);
	EXPECT_EQ(2, map["rel1"].atts["att2"]);
	EXPECT_EQ(3, map["rel1"].atts["att3"]);
	EXPECT_EQ(4, map["rel3"].atts["att4"]);
}

/**
 * Write should write "Relations\nAttribute\n" if the map is empty
 */
TEST_F(StatisticsTest, Write2) {
	StrictMock<MockRawFile> file;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, Append("Relations\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Append("Attributes\n")).
			WillOnce(Return(true));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));

	Write("file.txt", file);
}
