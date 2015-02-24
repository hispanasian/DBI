#include "DBFileTest.h"

/**
 * DBFileTest::Close should be tested with a mocked GenericDBFile
 */

/**
 * When close is called, DBFile should
 * - Flush its file delegate
 * - Close its delegate
 * - Write out its metadata file
 * - Close its metadata file
 * - Return 1 when successful
 */
TEST_F(DBFileTest, Close1) {
	StrictMock<MockGenericDBFile>& mockGeneric = *(new StrictMock<MockGenericDBFile>());
	SetDB(&mockGeneric);
	EXPECT_CALL(mockGeneric, Flush());
	EXPECT_CALL(mockGeneric, Reset());
	EXPECT_CALL(mockFile, Close()).
		WillOnce(Return(1));
	EXPECT_CALL(config, Write(Ref(rfile))).
		WillOnce(Return(true));
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(rfile, Close()).
		WillOnce(Return(true));


	ASSERT_EQ(1, file.Close());
	ASSERT_EQ(NULL, GetDB());
	// TODO: does not check that the memory was actually deallocated
}

/**
 * When close is called and config does not close properly,
 * the method should return 0
 */
TEST_F(DBFileTest, Close2) {
	StrictMock<MockGenericDBFile>& mockGeneric = *(new StrictMock<MockGenericDBFile>());
	SetDB(&mockGeneric);
	EXPECT_CALL(mockGeneric, Flush());
	EXPECT_CALL(mockGeneric, Reset());
	EXPECT_CALL(mockFile, Close()).
		WillOnce(Return(1));
	EXPECT_CALL(config, Write(Ref(rfile))).
		WillOnce(Return(false));
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(rfile, Close()).
		WillOnce(Return(true));


	ASSERT_EQ(0, file.Close());
	ASSERT_EQ(NULL, GetDB());
	// TODO: does not check that the memory was actually deallocated
}

/**
 * When close is called and the raw file does not close properly,
 * the method should return 0
 */
TEST_F(DBFileTest, Close3) {
	StrictMock<MockGenericDBFile>& mockGeneric = *(new StrictMock<MockGenericDBFile>());
	SetDB(&mockGeneric);
	EXPECT_CALL(mockGeneric, Flush());
	EXPECT_CALL(mockGeneric, Reset());
	EXPECT_CALL(mockFile, Close()).
		WillOnce(Return(1));
	EXPECT_CALL(config, Write(Ref(rfile))).
		WillOnce(Return(true));
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(rfile, Close()).
		WillOnce(Return(false));


	ASSERT_EQ(0, file.Close());
	ASSERT_EQ(NULL, GetDB());
	// TODO: does not check that the memory was actually deallocated
}

/**
 * When close is called and the config and raw file do not close properly,
 * the method should return 0
 */
TEST_F(DBFileTest, Close4) {
	StrictMock<MockGenericDBFile>& mockGeneric = *(new StrictMock<MockGenericDBFile>());
	SetDB(&mockGeneric);
	EXPECT_CALL(mockGeneric, Flush());
	EXPECT_CALL(mockGeneric, Reset());
	EXPECT_CALL(mockFile, Close()).
		WillOnce(Return(1));
	EXPECT_CALL(config, Write(Ref(rfile))).
		WillOnce(Return(false));
	EXPECT_CALL(config, Clear());
	EXPECT_CALL(rfile, Close()).
		WillOnce(Return(false));


	ASSERT_EQ(0, file.Close());
	ASSERT_EQ(NULL, GetDB());
	// TODO: does not check that the memory was actually deallocated
}