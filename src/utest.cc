#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "DBFile.h"
#include "MockClasses.h"

using ::testing::_;

TEST(DBFile, Test) {
  DBFile f;
  MockRecord mr;
  ASSERT_EQ(0, f.Close());

  EXPECT_CALL(mr, Project(_,_,_));

  f.Add(mr);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
