#include "DBFileTest.h"

/**
 * DBFile::FileExists should return false when no file named "asdasdasd" is found.
 */
TEST(DBFile, FileExists1) {
	DBFile file;
	char *path = "asdasdasd";
	EXPECT_EQ(false, FileExists(path));
}

/**
 * DBFile::FileExists should return true when a file named "asdasdasd" exists.
 */
TEST(DBFile, FileExists2) {
	DBFile file;
	char *path = "asdasdasd";
	FILE *temp = fopen(path, "w");
	fprintf(temp, "stuff");
	fclose(temp);
	EXPECT_EQ(true, FileExists(path));
	remove(path);
}
