#ifndef OPNODETEST_H
#define OPNODETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "OpNode.h"
#include "Schema.h"
#include "Defs.h"
#include "SQL.h"
#include "Statistics.h"
#include <string>

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
using ::testing::SetArgReferee;
using ::testing::DoAll;

class PartialProjectNode: public ProjectNode {
public:
	PartialProjectNode(int id, OpNode *_child, const std::vector<RelAttPair> &_attsToKeep):
		ProjectNode(id, _child, _attsToKeep) {}
	MOCK_METHOD0(ContainsAggregate, bool());
};

class OpNodeTest: public ::testing::Test {
public:
	Schema A;
	Schema B;
	Schema C;
	Schema D;
	Schema E;
	Schema childSchema;
	Schema emptySchema;

	Attribute *atts = new Attribute[9];
	Attribute *attsA = new Attribute[3];
	Attribute *attsB = new Attribute[2];
	Attribute *attsC = new Attribute[1];
	Attribute *attsD = new Attribute[1];
	Attribute *attsE = new Attribute[2];

	MockOpNode child;
	Statistics stats;
	std::string query;

	OpNodeTest() {
		Attribute *temp;
		// Make Schema
		temp = &atts[0]; temp->relation="A"; temp->name="a"; temp->myType=Int;
		temp = &atts[1]; temp->relation="A"; temp->name="b"; temp->myType=Int;
		temp = &atts[2]; temp->relation="A"; temp->name="c"; temp->myType=Int;
		temp = &atts[3]; temp->relation="B"; temp->name="a"; temp->myType=Double;
		temp = &atts[4]; temp->relation="B"; temp->name="b"; temp->myType=Double;
		temp = &atts[5]; temp->relation="C"; temp->name="c"; temp->myType=String;
		temp = &atts[6]; temp->relation="D"; temp->name="d"; temp->myType=String;
		temp = &atts[7]; temp->relation="E"; temp->name="e"; temp->myType=Double;
		temp = &atts[8]; temp->relation="E"; temp->name="b"; temp->myType=Double;

		childSchema.Copy(Schema("", 9, atts));

		temp = &attsA[0]; temp->relation="A"; temp->name="a"; temp->myType=Int;
		temp = &attsA[1]; temp->relation="A"; temp->name="b"; temp->myType=Int;
		temp = &attsA[2]; temp->relation="A"; temp->name="c"; temp->myType=Int;
		A.Copy(Schema("", 3, attsA));

		temp = &attsB[0]; temp->relation="B"; temp->name="a"; temp->myType=Double;
		temp = &attsB[1]; temp->relation="B"; temp->name="b"; temp->myType=Double;
		B.Copy(Schema("", 2, attsB));

		temp = &attsC[0]; temp->relation="C"; temp->name="c"; temp->myType=String;
		C.Copy(Schema("", 1, attsC));

		temp = &attsD[0]; temp->relation="D"; temp->name="d"; temp->myType=String;
		D.Copy(Schema("", 1, attsD));

		temp = &attsE[0]; temp->relation="E"; temp->name="e"; temp->myType=Double;
		temp = &attsE[1]; temp->relation="E"; temp->name="b"; temp->myType=Double;
		E.Copy(Schema("", 2, attsE));


		emptySchema.Copy(Schema("", 0, new Attribute[0]));

		// Make Statistics
		stats.AddRel("RelA", 0);
		stats.AddRel("RelB", 0);
		stats.AddRel("RelC", 0);
		stats.AddRel("RelD", 0);
		stats.AddRel("RelE", 0);

		stats.AddAtt("RelA", "a", 0);
		stats.AddAtt("RelA", "b", 0);
		stats.AddAtt("RelA", "c", 0);
		stats.AddAtt("RelB", "a", 0);
		stats.AddAtt("RelB", "b", 0);
		stats.AddAtt("RelC", "c", 0);
		stats.AddAtt("RelD", "d", 0);
		stats.AddAtt("RelE", "e", 0);
		stats.AddAtt("RelE", "b", 0);

		// Make SQL
		query = "SELECT SUM DISTINCT(A.b + 3 + B.b), E.e, A.a, B.a, C.c ";
		query.append(" FROM RelA AS A, RelB AS B, RelC AS C, RelD AS D, RelE AS E ");
		query.append(" WHERE (A.a = 5 OR A.c = 6) AND (A.a = B.b) AND (B.a = C.c) AND (C.c = D.d) AND (D.d = E.e) ");
		query.append(" GROUP BY A.a, B.a ");
	}
};

#endif
