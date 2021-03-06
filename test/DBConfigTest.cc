#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/DBConfig.h"
#include "../include/MockClasses.h"
#include <map>

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;

/**
* Fixture for DBConfig that provides access to map.
*/
class DBConfigTest: public ::testing::Test {
public:
    DBConfig config;
    map<std::string,std::string>& Map();
protected:
    //virtual void SetUp() {}
    //virtual void TearDown() {}
};

/**
* Returns the map of config
*/
map<std::string,std::string>& DBConfigTest::Map() {
    return config.map;
}

/***************************************************************
 ************************** Unit tests *************************
 ***************************************************************/

/**
* DBConfig::Read should take an open RawFile that has no contents (or simply an un-opened RawFile),
* read it once (via readline). If Close poses no problem, then Read return true and DBConfig should
* have an empty map. It should also call RawFile::LSeek(0) before it starts.
*/
TEST_F(DBConfigTest, Read1) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, LSeek(0));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(Return(false));
    }
    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(true, Map().empty());
}

/**
* DBConfig::Read should take an open RawFile that starts at the beginning with 3 elements correctly
* formatted and add them to map. It should then add them to the map and finally return true. It
* should also call RawFile::LSeek(0) to read from the beginning.
*/
TEST_F(DBConfigTest, Read2) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, LSeek(0));

        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("key=value"), Return(true)));

        EXPECT_CALL(file, ReadLine(_)).
            WillOnce(DoAll(
            testing::SetArgPointee<0>("this=that"), Return(true)));

        EXPECT_CALL(file, ReadLine(_)).
            WillOnce(DoAll(
            testing::SetArgPointee<0>("kit=kat"), Return(true)));

        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(Return(false));
    }
    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(3, Map().size());

    std::string value("value");
    std::string that("that");
    std::string kat("kat");
    EXPECT_EQ(0, value.compare(Map()["key"]));
    EXPECT_EQ(0, that.compare(Map()["this"]));
    EXPECT_EQ(0, kat.compare(Map()["kit"]));
}

/**
* DBCofnig::Read should return true if the file was empty. It should also call RawFile::LSeek(0)
* to read from the beginning.
*/
TEST_F(DBConfigTest, Read3) {
    MockRawFile file;
    {
    	EXPECT_CALL(file, LSeek(0));
        InSequence seq;
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(Return(false));
    }
    EXPECT_EQ(true, config.Read(file));
}

/**
* DBConfig::Read should return false if it encounters a line that does not match the format:
* "key=value". It should also call RawFile::LSeek(0) to read from the beginning.
*/
TEST_F(DBConfigTest, Read4) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, LSeek(0));
        /* Some valid lines */
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("key=val"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("good=not bad"), Return(true)));
        /* Invalid line */
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("invalid"), Return(true)));
    }

    EXPECT_EQ(false, config.Read(file));
}

/**
* DBConfig::Read should return true if it encounters multiple keys. It should over-write the old
* key in the file. It should also call RawFile::LSeek(0) to read from the beginning.
*/
TEST_F(DBConfigTest, Read5) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, LSeek(0));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("a=A"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("yes=no"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("check=good"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("check=bad"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("no=yes"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(Return(false));
    }

    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(4, Map().size());

    std::string A("A");
    std::string no("no");
    std::string good("good");
    std::string yes("yes");
    EXPECT_EQ(0, A.compare(Map()["a"]));
    EXPECT_EQ(0, no.compare(Map()["yes"]));
    EXPECT_EQ(0, good.compare(Map()["check"]));
    EXPECT_EQ(0, yes.compare(Map()["no"]));
}

/**
* DBConfig::Read should not fail if it finds an extra '=' character. The first '=' character will
* act as the delimiter for key,value and any remaining '=' characters will be used as part of val.
* It should also call RawFile::LSeek(0) to read from the beginning.
*/
TEST_F(DBConfigTest, Read6) {
    MockRawFile file;
    {
        InSequence seq;
        EXPECT_CALL(file, LSeek(0));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("non=sense"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("this=is=dumb"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(DoAll(
                testing::SetArgPointee<0>("not=dumb"), Return(true)));
        EXPECT_CALL(file, ReadLine(_)).
                WillOnce(Return(false));
    }

    EXPECT_EQ(true, config.Read(file));
    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(3, Map().size());

    std::string sense("sense");
    std::string isDumb("is=dumb");
    std::string dumb("dumb");
    EXPECT_EQ(0, sense.compare(Map()["non"]));
    EXPECT_EQ(0, isDumb.compare(Map()["this"]));
    EXPECT_EQ(0, dumb.compare(Map()["not"]));
}

/**
* DBConfig::Write should first call Truncate on RawFile followed by a number of Appends equal to
* the number of elements in Map. This should also add anew line after every string when calling
* RawFile::Append. It should also call RawFile::LSeek(0) after truncate to write at the beginning.
*/
TEST_F(DBConfigTest, Write1) {
    MockRawFile file;
    Map().insert(std::pair<std::string, std::string>("key", "val"));
    Map().insert(std::pair<std::string, std::string>("get", "stuffed"));
    Map().insert(std::pair<std::string, std::string>("one", "liners"));

    Sequence s1, s2, s3;
    EXPECT_CALL(file, Truncate()).
            InSequence(s1, s2, s3).
            WillOnce(Return(true));
    EXPECT_CALL(file, LSeek(0)).
    		InSequence(s1, s2, s3);
    /* Not sure which order these should appear in */
    EXPECT_CALL(file, Append("key=val\n")).
            InSequence(s1).
            WillOnce(Return(true));
    EXPECT_CALL(file, Append("get=stuffed\n")).
            InSequence(s2).
            WillOnce(Return(true));
    EXPECT_CALL(file, Append("one=liners\n")).
            InSequence(s3).
            WillOnce(Return(true));

    EXPECT_EQ(true, config.Write(file));
}

/**
* DBConfig::Write should stop writing to file as soon as Append returns false. It should also call
* RawFile::LSeek(0) after truncate to write at the beginning.
*/
TEST_F(DBConfigTest, Write2) {
    MockRawFile file;
    Map().insert(std::pair<std::string, std::string>("key", "val"));
    Map().insert(std::pair<std::string, std::string>("get", "stuffed"));
    Map().insert(std::pair<std::string, std::string>("one", "liners"));

    {
        InSequence seq;
        EXPECT_CALL(file, Truncate()).
                WillOnce(Return(true));
        EXPECT_CALL(file, LSeek(0));
        EXPECT_CALL(file, Append(_)).
                WillOnce(Return(true));
        EXPECT_CALL(file, Append(_)).
                WillOnce(Return(false));
    }

    EXPECT_EQ(false, config.Write(file));
}

/**
* DBConfig::Write should return false if file.Truncate returns false. DBConfig should not try to
* append anything to file but it should also try to close the file. It should not call LSeek(0)
* after truncate.
*/
TEST_F(DBConfigTest, Write3) {
    MockRawFile file;
    Map().insert(std::pair<std::string, std::string>("key", "val"));
    Map().insert(std::pair<std::string, std::string>("get", "stuffed"));
    Map().insert(std::pair<std::string, std::string>("one", "liners"));

    {
        InSequence seq;
        EXPECT_CALL(file, Truncate()).
                WillOnce(Return(false));
    }

    EXPECT_EQ(false, config.Write(file));
}

/**
* DBConfig::Write should return false if file.Truncate returns false. DBConfig should not try to
* append anything to file. It should not call LSeek if Truncate fails.
*/
TEST_F(DBConfigTest, Write4) {
    MockRawFile file;
    Map().insert(std::pair<std::string, std::string>("key", "val"));
    Map().insert(std::pair<std::string, std::string>("get", "stuffed"));
    Map().insert(std::pair<std::string, std::string>("one", "liners"));

    EXPECT_CALL(file, Truncate()).
                    WillOnce(Return(false));

    EXPECT_EQ(false, config.Write(file));
}

/**
* DBConfig::AddKey should add a key-value pair to DBConfig::map when AddKey is called.
*/
TEST_F(DBConfigTest, AddKey1) {
    config.AddKey("key", "val");
    config.AddKey("this", "is");
    config.AddKey("a", "test");

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(3, Map().size());

    std::string val("val");
    std::string is("is");
    std::string test("test");
    EXPECT_EQ(0, val.compare(Map()["key"]));
    EXPECT_EQ(0, is.compare(Map()["this"]));
    EXPECT_EQ(0, test.compare(Map()["a"]));
}

/**
* DBConfig::AddKey should not overwrite an existing key
*/
TEST_F(DBConfigTest, AddKey2) {
    config.AddKey("key", "val");
    config.AddKey("key", "this cant happen");
    config.AddKey("a", "test");

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(2, Map().size());

    std::string val("val");
    std::string test("test");
    EXPECT_EQ(0, val.compare(Map()["key"]));
    EXPECT_EQ(0, test.compare(Map()["a"]));
}

/**
* DBConfig::AddKey should throw an exception if the key contains the '=' character
*/
TEST_F(DBConfigTest, AddKey3) {
    config.AddKey("key", "val");
    ASSERT_THROW(config.AddKey("not=valid", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::AddKey should throw an exception if the key contains the '\n' character
*/
TEST_F(DBConfigTest, AddKey4) {
    config.AddKey("key", "val");
    ASSERT_THROW(config.AddKey("not\n", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::AddKey should throw an exception if the key contains the '\r' character
*/
TEST_F(DBConfigTest, AddKey5) {
    config.AddKey("key", "val");
    ASSERT_THROW(config.AddKey("not\r", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::AddKey should throw an exception if the value contains the '\n' character
*/
TEST_F(DBConfigTest, AddKey6) {
    config.AddKey("key", "val");
    ASSERT_THROW(config.AddKey("not", "invalid\n"), IllegalValueException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::AddKey should throw an exception if the key contains the '\r' character
*/
TEST_F(DBConfigTest, AddKey7) {
    config.AddKey("key", "val");
    ASSERT_THROW(config.AddKey("not", "invalid\r"), IllegalValueException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::GetKey should return the value with an associated key if it exists
*/
TEST_F(DBConfigTest, GetKey1) {
    Map().insert(pair<string, string>("key", "value"));
    std::string value = "value";

    EXPECT_EQ(0, value.compare(config.GetKey("key")));
}

/**
* DBConfig::GetKey should return "" if a key does not exist and it should not add the non-existent
* key to map.
*/
TEST_F(DBConfigTest, GetKey2) {
    std::string value = "";
    EXPECT_EQ(0, value.compare(config.GetKey("key")));
    EXPECT_EQ(0, Map().size());
}

/**
 * DBConfig::ReplaceKey should, if it has no elements, map value to key
 */
TEST_F(DBConfigTest, ReplaceKey1) {
	config.ReplaceKey("key", "value");
	std::string value = "value";
	EXPECT_EQ(0, value.compare(config.GetKey("key")));
	EXPECT_EQ(1, Map().size());
}

/**
 * DBConfig::ReplaceKey should, if it has a map to key exists, replace the old value withh the new
 * one.
 */
TEST_F(DBConfigTest, ReplaceKey2) {
	Map().insert(pair<string, string>("key", "value"));
	config.ReplaceKey("key", "newval");
	std::string newval = "newval";
	EXPECT_EQ(0, newval.compare(config.GetKey("key")));
	EXPECT_EQ(1, Map().size());
}

/**
* DBConfig::ReplaceKey should throw an exception if the key contains the '=' character
*/
TEST_F(DBConfigTest, ReplaceKey3) {
	Map().insert(pair<string, string>("key", "val"));
    ASSERT_THROW(config.ReplaceKey("not=valid", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::ReplaceKey should throw an exception if the key contains the '\n' character
*/
TEST_F(DBConfigTest, ReplaceKey4) {
	Map().insert(pair<string, string>("key", "val"));
    ASSERT_THROW(config.ReplaceKey("not\n", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::ReplaceKey should throw an exception if the key contains the '\r' character
*/
TEST_F(DBConfigTest, ReplaceKey5) {
	Map().insert(pair<string, string>("key", "val"));
    ASSERT_THROW(config.ReplaceKey("not\r", "invalid"), IllegalKeyException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::ReplaceKey should throw an exception if the value contains the '\n' character
*/
TEST_F(DBConfigTest, ReplaceKey6) {
	Map().insert(pair<string, string>("key", "val"));
    ASSERT_THROW(config.ReplaceKey("not", "invalid\n"), IllegalValueException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::ReplaceKey should throw an exception if the key contains the '\r' character
*/
TEST_F(DBConfigTest, ReplaceKey7) {
	Map().insert(pair<string, string>("key", "val"));
    ASSERT_THROW(config.ReplaceKey("not", "invalid\r"), IllegalValueException);

    EXPECT_EQ(false, Map().empty());
    EXPECT_EQ(1, Map().size());

    std::string val("val");
    EXPECT_EQ(0, val.compare(Map()["key"]));
}

/**
* DBConfig::Clear should remove all the elements in map.
*/
TEST_F(DBConfigTest, Clear1) {
	Map().insert(pair<string, string>("key", "val"));
	Map().insert(pair<string, string>("key2", "val"));
	Map().insert(pair<string, string>("key3", "val"));
	Map().insert(pair<string, string>("key4", "val"));

    config.Clear();
    EXPECT_EQ(0, Map().size());
}

/**
* DBConfig::Clear should not crash attempting to clear an empty map.
*/
TEST_F(DBConfigTest, Clear2) {
    config.Clear();
    EXPECT_EQ(0, Map().size());
}

/***************************************************************
 ********************** Integration tests **********************
 ***************************************************************/
/**
 * DBConfigTest should correctly write to a file.
 */
TEST_F(DBConfigTest, IntegrationTest1) {
	RawFile file;
	file.Open("jkdlkjfslkdjfsdf");
	config.AddKey("test","val");
	config.Write(file);

	FILE *check = fopen("jkdlkjfslkdjfsdf", "r");
	char space[200];
	fscanf(check, "%s", space);
	EXPECT_EQ(0, strcmp("test=val", space));
	fclose(check);
	remove("jkdlkjfslkdjfsdf");
}

/**
 * DBConfigTest should correctly read from a file.
 */
TEST_F(DBConfigTest, IntegrationTest2) {
	FILE * check = fopen("jkdlkjfslkdjfsdf", "w+");
	fprintf (check, "key=val");
	fclose(check);
	RawFile file;
	file.Open("jkdlkjfslkdjfsdf");
	EXPECT_EQ(true, config.Read(file));
	string val = "val";
	EXPECT_EQ(0, val.compare(config.GetKey("key")));
	file.Close();
	remove("jkdlkjfslkdjfsdf");
}
