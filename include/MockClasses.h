#include <gmock/gmock.h>
#include "Record.h"
#include "File.h"
#include "RawFile.h"
#include "Config.h"

class MockRecord: public Record {
public:
    // void Consume (Record *fromMe);
    MOCK_METHOD1(Consume, void(Record *fromMe));
    // void Copy (Record *copyMe);
    MOCK_METHOD1(Copy, void(Record *copyMe));
    // int SuckNextRecord (Schema *mySchema, FILE *textFile);
    MOCK_METHOD2(SuckNextRecord, int(Schema *mySchema, FILE *textFile));
    // void Project (int *attsToKeep, int numAttsToKeep, int numAttsNow);
    MOCK_METHOD3(Project, void(int *attsToKeep, int numAttsToKeep, int numAttsNow));
    // void MergeRecords (Record *left, Record *right, int numAttsLeft,
    // int numAttsRight, int *attsToKeep, int numAttsToKeep, int startOfRight);
    MOCK_METHOD7(MergeRecords, void(Record *left, Record *right, int numAttsLeft,
    int numAttsRight, int *attsToKeep, int numAttsToKeep, int startOfRight));
    // void Print (Schema *mySchema);
    MOCK_METHOD1(Print, void(Schema *mySchema));
};

class MockPage: public Page {
public:
    //virtual void ToBinary (char *bits);
    MOCK_METHOD1(ToBinary, void(char *bits));
    //virtual void FromBinary (char *bits);
    MOCK_METHOD1(FromBinary, void(char *bits));
    //virtual int GetFirst (Record *firstOne);
    MOCK_METHOD1(GetFirst, int(Record *firstOne));
    //virtual int Append (Record *addMe);
    MOCK_METHOD1(Append, int(Record *addMe));
    //virtual void EmptyItOut ();
    MOCK_METHOD0(EmptyItOut, void());
};

class MockFile: public File {
public:
    //virtual void Open (int length, char *fName);
    MOCK_METHOD2(Open, void(int length, char *fname));
    //virtual void GetPage (Page *putItHere, off_t whichPage);
    MOCK_METHOD2(GetPage, void(Page *putItHere, off_t whichPage));
    //virtual void AddPage (Page *addMe, off_t whichPage);
    MOCK_METHOD2(AddPage, void(Page *addMe, off_t whichPage));
    //virtual int Close ();
    MOCK_METHOD0(Close, int());
};

class MockRawFile: public RawFile {
public:
//    virtual bool Open(std::string fname);
    MOCK_METHOD1(Open, bool(std::string fname));
//    virtual bool Close();
    MOCK_METHOD0(Close, bool());
//    virtual int Read(void* buf, size_t count);
    MOCK_METHOD2(Read, int(void* buf, size_t count));
//    virtual int Write(const void* buf, size_t count);
    MOCK_METHOD2(Write, int(const void* buf, size_t count));
//    virtual std::string ReadLine();
    MOCK_METHOD0(ReadLine,  std::string());
//    virtual bool Append(std::string value);
    MOCK_METHOD1(Append, bool(std::string value));
//    virtual bool Truncate();
    MOCK_METHOD0(Truncate, bool());
//    virtual void LSeek(off_t offset);
    MOCK_METHOD1(LSeek, void(off_t offset));
};

class MockConfig: public Config {
public:
//    virtual bool Open(std::string fname);
    MOCK_METHOD1(Open, bool(std::string fname));
//    virtual bool Close();
    MOCK_METHOD0(Close, bool());
//    virtual void AddKey(std::string key, std::string value);
    MOCK_METHOD2(AddKey, void(std::string key, std::string value));
//    virtual std::string GetKey(std::string key);
    MOCK_METHOD1(GetKey, std::string(std::string key));
};
