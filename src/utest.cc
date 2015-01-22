#include <gtest/gtest.h>
#include "DBFile.h"

TEST(Test, Test) {
  ASSERT_EQ(true, true);
}

TEST(DBFile, Test) {
  DBFile f = *(new DBFile());
  ASSERT_EQ(0, f.Close());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
