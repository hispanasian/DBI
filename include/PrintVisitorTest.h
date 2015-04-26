#ifndef PRINTVISITORTEST_H 
#define PRINTVISITORTEST_H 

#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" struct YY_BUFFER_STATE *yysql_scan_string(const char*);
extern "C" int yyparse(void);
extern "C" int yysqlparse(void);
extern struct AndList *final;
extern struct NameList *attsToSelect;

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

class PrintVisitorTest: public::testing::Test {

};

#endif /* PRINTVISITORTEST_H*/
