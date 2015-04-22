#ifndef JOINOPTIMIZERTEST_H
#define JOINOPTIMIZERTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Statistics.h"
#include "MockClasses.h"
#include "ParseTree.h"
#include "JoinOptimizer.h"

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

class JoinOptimizerTest: public::testing::Test {
	JoinOptimizer opt;
public:
    bool SetsEqual(const vector<bool>& set1, const vector<bool>& set2) {
    	return opt.SetsEqual(set1, set2);
    } 

    void Indices(const vector<bool>& set, vector<int>& inidices) {
    	return opt.Indices(set, inidices);
    }
    AndList* GetAndList(const int index, const vector<int>& indices,
                    const vector<string>& relNames,
                    const unordered_map<string, unordered_map<string, AndList*> > &joins) {
    	return opt.GetAndList(index, indices, relNames, joins);
    }
    const void GetRelNames(const int index, const vector<int>& indices, const char* names[],
                    const vector<string>& relNames) {
    	return opt.GetRelNames(index, indices, names, relNames);
    }
  
};

#endif /* JOINOPTIMIZERTEST_H*/
