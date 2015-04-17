/*
 * SQLTest.h
 *
 *  Created on: Apr 2, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SQLTEST_H_
#define INCLUDE_SQLTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SQL.h"
#include "Statistics.h"
#include "MockClasses.h"
#include "ParseTree.h"

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

class SQLTest: public::testing::Test {
public:
	Statistics stat;
};

#endif /* INCLUDE_SQLTEST_H_ */
