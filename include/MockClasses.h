#include <gmock/gmock.h>
#include "Record.h"
#include "File.h"

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