
#ifndef SCHEMA_H
#define SCHEMA_H

#include <stdio.h>
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "SQL.h"
#include <iostream>;
#include <vector>

struct Attribute {
	std::string name;
	std::string relation;
	Type myType;
};

class OrderMaker;
class Schema {

	friend class RecordTest;
	friend class SchemaTest;

	// gives the attributes in the schema
	int numAtts;
	Attribute *myAtts;

	// gives the physical location of the binary file storing the relation
	char *fileName;

	friend class Record;
	friend class DBFileTest;
	friend class MockSchema;

public:

	// gets the set of attributes, but be careful with this, since it leads
	// to aliasing!!!
	Attribute *GetAtts ();

	// returns the number of attributes
	int GetNumAtts ();

	// this finds the position of the specified attribute in the schema
	// returns a -1 if the attribute is not present in the schema
	int Find (const char *attName) const;

	// this finds the position of the specified relation/attribute in the schema
	// returns a -1 if the attribute is not present in the schema
	int Find (const char *relName, const char *attName) const;

	// this finds the type of the given attribute
	Type FindType (const char *attName) const;

	// this finds the type of the given relation/attribute
	Type FindType (const char *relName, const char *attName) const;

	// this reads the specification for the schema in from a file
	Schema (char *fName, char *relName);

	// this composes a schema instance in-memory
	Schema (char *fName, int num_atts, Attribute *atts);

	// Copy Constructor
	Schema (const Schema &copyMe);

	// Constructs a Schema out of copyMe and the vector of Relation/Attributes pairs. The
	// Relation/Attributes pairs are expected to be a subset of copyMe. The new schema will
	// simply be a transformation of the vector into a Schema with copyMe provided to give the
	// type information of each Relation/Attribute pair
	Schema (const Schema &copyMe, const std::vector<RelAttPair> &pairs);

	// This effectively makes a copy of left and right and "joins" them
	// into this schema. left will be the first set of attributes in this
	// followed by right
	Schema (Schema *left, Schema *right);

	// Joins the two relations into this schema
	void Join(Schema *left, Schema *right);

	// this constructs a sort order structure that can be used to
	// place a lexicographic ordering on the records using this type of schema
	int GetSortOrder (OrderMaker &order);

	// sets the relation of every relation in this Schema to the provided relation
	void SetRelation(const char* relation);

	virtual ~Schema ();

	// Strictly for testing
	Schema() {
		fileName = NULL;
		numAtts = NULL;
		myAtts = NULL;
	}

};

#endif
