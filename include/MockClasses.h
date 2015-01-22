#include <gmock/gmock.h>
#include "Record.h"

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
