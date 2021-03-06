/*
 * SQLEngineTest.h
 *
 *  Created on: Apr 22, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_SQLENGINETEST_H_
#define INCLUDE_SQLENGINETEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SQL.h"
#include "SQLEngineTest.h"
#include "Statistics.h"
#include "MockClasses.h"
#include "RelationData.h"
#include "SQLEngine.h"

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
using ::testing::StrEq;

class SQLEngineTest: public::testing::Test {
public:
	Statistics stat;
	RelationData relations;

	SQLEngineTest();
	~SQLEngineTest();

	void CreateTable(SQLEngine &eng, SQL *sql, std::vector<AttTypePair> *atts, std::vector<std::string> *order,
				std::string tableName, DB_Type type, DBFile &db);

	virtual void Insert(SQLEngine &eng, SQL *sql, std::string file, std::string table, DBFile &db);

	virtual void DropTable(SQLEngine &eng, SQL *sql, std::string table, RawFile &rfile);
};

#endif /* INCLUDE_SQLENGINETEST_H_ */
