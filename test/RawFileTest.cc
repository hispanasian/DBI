#include <gtest/gtest.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RawFile.h"

using namespace std;

TEST(RawFile, Read) {
    char buff[1024];
    for(int i = 0; i < sizeof(buff); ++i) {
        buff[i] = '\0';
    }
    int ret = system("echo line1 > rawfile.txt");
    ASSERT_EQ(0, ret);
    ret = system("echo line2 >> rawfile.txt");
    ASSERT_EQ(0, ret);

    RawFile f;
    bool result = f.Open("rawfile.txt");
    ASSERT_EQ(true, result);
    int bytes = f.Read(buff, sizeof(buff));
    ASSERT_EQ(12, bytes);

    result = f.Close();
    ASSERT_EQ(true, result);
    ret = system("rm rawfile.txt");
    ASSERT_EQ(0, ret);
}

TEST(RawFile, Write) {
    string fname = "rawfile.txt";
    string cmd = "touch " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);

    char contents[] = "file contents";
    char buff[sizeof(contents)];
    for(int i = 0; i < sizeof(buff); ++i) {
        buff[i] = '\0';
    }
    RawFile f;
    bool result = f.Open("rawfile.txt");
    ASSERT_EQ(true, result);
    int bytes = f.Write(contents, sizeof(contents));
    result = f.Close();
    ASSERT_EQ(true, result);

    FILE* cat = popen("cat rawfile.txt", "r");
    bytes = fread(buff, 1, sizeof(buff), cat);
    ASSERT_EQ(14, bytes);
    ASSERT_STREQ(contents, buff);
    pclose(cat);
    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}
