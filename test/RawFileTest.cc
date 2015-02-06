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

TEST(RawFile, ReadLine1) {
    string fname = "rawfile.txt";
    string cmd = "echo line1 > " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
    cmd = "echo line2 >> " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);

    string* line = new string();
    RawFile f;
    bool result = f.Open(fname);
    ASSERT_EQ(true, result);

    result = f.ReadLine(line);
    ASSERT_EQ(true, result);
    ASSERT_STREQ("line1", line->c_str());
    result = f.ReadLine(line);
    ASSERT_EQ(true, result);
    ASSERT_STREQ("line2", line->c_str());
    result = f.ReadLine(line);
    ASSERT_EQ(false, result);

    delete line;

    result = f.Close();
    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}


// If there are no newlines in the file,
// RawFile should return the contents of the file
// as the one and only line.
TEST(RawFile, ReadLine2) {
    string fname = "rawfile.txt";
    FILE * check = fopen(fname.c_str(), "w+");
    fprintf(check, "key=val");
    fclose(check);

    RawFile f;
    bool result = f.Open(fname);
    string* line = new string();
    ASSERT_EQ(true, result);
    result = f.ReadLine(line);
    ASSERT_EQ(true, result);
    ASSERT_STREQ("key=val", line->c_str());
    result = f.ReadLine(line);
    ASSERT_EQ(false, result);

    result = f.Close();
    ASSERT_EQ(true, result);

    delete line;
    string cmd = "rm " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}

// empty files should have 0 lines
TEST(RawFile, ReadLine3) {
    string fname = "rawfile.txt";
    string cmd = "touch " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);

    string* line = new string();
    RawFile f;
    bool result = f.Open(fname);
    ASSERT_EQ(true, result);
    result = f.ReadLine(line);
    ASSERT_EQ(false, result);
    result = f.Close();
    ASSERT_EQ(true, result);

    delete line;
    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}


TEST(RawFile, Append) {
    char buff[1024];
    for(int i = 0; i < sizeof(buff); ++i) {
        buff[i] = '\0';
    }
    string fname = "rawfile.txt";
    string cmd = "rm " + fname;
    system(cmd.c_str());
    cmd = "touch " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);


    RawFile f;
    bool result = f.Open("rawfile.txt");
    ASSERT_EQ(true, result);
    result = f.Append("Line 1\n");
    ASSERT_EQ(true, result);
    result = f.Append("Line 2\n");
    ASSERT_EQ(true, result);

    result = f.Close();
    ASSERT_EQ(true, result);

    FILE* cat = popen("cat rawfile.txt", "r");
    int bytes = fread(buff, 1, sizeof(buff), cat);
    ASSERT_EQ(14, bytes);
    ASSERT_STREQ("Line 1\nLine 2\n", buff);
    pclose(cat);

    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}

TEST(RawFile, Truncate) {
    char buff[1024];
    for(int i = 0; i < sizeof(buff); ++i) {
        buff[i] = '\0';
    }
    string fname = "rawfile.txt";
    string cmd = "echo line1 > " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);

    RawFile f;
    bool result = f.Open(fname);
    ASSERT_EQ(true, result);
    result = f.Truncate();
    ASSERT_EQ(true, result);
    result = f.Close();
    ASSERT_EQ(true, result);

    FILE* cat = popen("cat rawfile.txt", "r");
    int bytes = fread(buff, 1, sizeof(buff), cat);
    ASSERT_EQ(0, bytes);
    pclose(cat);

    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}

TEST(RawFile, LSeek) {
    char buff[1024];
    for(int i = 0; i < sizeof(buff); ++i) {
        buff[i] = '\0';
    }
    string fname = "rawfile.txt";
    string cmd = "echo sometext > " + fname;
    int ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);

    RawFile f;
    bool result = f.Open(fname);
    ASSERT_EQ(true, result);
    f.LSeek(4);
    result = f.Write("some", 4);
    f.LSeek(0);
    result = f.Write("text", 4);
    ASSERT_EQ(true, result);
    result = f.Close();
    ASSERT_EQ(true, result);

    FILE* cat = popen("cat rawfile.txt", "r");
    int bytes = fread(buff, 1, sizeof(buff), cat);
    ASSERT_EQ(9, bytes);
    ASSERT_STREQ("textsome\n", buff);
    pclose(cat);

    cmd = "rm " + fname;
    ret = system(cmd.c_str());
    ASSERT_EQ(0, ret);
}
