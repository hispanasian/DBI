#ifndef COMPARISON_H
#define COMPARISON_H

#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include <string>


// This stores an individual comparison that is part of a CNF
class Comparison {

	friend class ComparisonEngine;
	friend class CNF;
	friend class CNFTest;

	Target operand1;
	int whichAtt1;
	Target operand2;
	int whichAtt2;

	Type attType;

	CompOperator op;

public:

	Comparison();

	//copy constructor
	Comparison(const Comparison &copyMe);

	// print to the screen
	void Print ();

	std::string ToString(); 
};


class Schema;

// This structure encapsulates a sort order for records
class OrderMaker {

	friend class ComparisonEngine;
	friend class CNF;
	friend class OrderMakerTest;

	int numAtts;

	int whichAtts[MAX_ANDS];
	Type whichTypes[MAX_ANDS];

public:
	
	// creates an empty OrdermMaker
	OrderMaker();

	// create an OrderMaker that can be used to sort records
	// based upon ALL of their attributes
	OrderMaker(Schema *schema);

	// create an OrderMaker that can be used to sort records
	// based on the attributes in order (relative to base).
	// ie: OrderMaker will sort on the attributes provided by
	// order but will use the context of base
	OrderMaker(Schema *base, Schema *order);

	// create an OrderMaker that can be used to sort records
	// based upon str which is expected to have the following format:
	// OrderMaker = (whichAtt whichType)*
	OrderMaker(std::string str);

	virtual ~OrderMaker();

	// gets the number of attributes
	virtual int GetNumAtts();

	// gets the index of the ith attribute
	virtual int GetAttIndex(int i);

	// get the Type of the ith attribute
	virtual Type GetAttType(int i);

	// print to the screen
	virtual void Print ();

	// returns a string representation of OrderMaker in the following format:
	// OrderMaker = (whichAtt whichType)*
	virtual std::string ToString();
};

class Record;

// This structure stores a CNF expression that is to be evaluated
// during query execution

class CNF {

	friend class ComparisonEngine;
	friend class CNFTest;

	Comparison orList[MAX_ANDS][MAX_ORS];
	
	int orLens[MAX_ANDS];
	int numAnds;

public:
	virtual ~CNF();
	// this returns an instance of the OrderMaker class that
	// allows the CNF to be implemented using a sort-based
	// algorithm such as a sort-merge join.  Returns a 0 if and
	// only if it is impossible to determine an acceptable ordering
	// for the given comparison
	virtual int GetSortOrders (OrderMaker &left, OrderMaker &right);

	virtual int GetSortOrder (OrderMaker &order);

	// Looks for the attributes in sortOrder that can be sorted on based on this CNF and puts
	// it in query.
	virtual bool MakeQuery(const OrderMaker &sortOrder, OrderMaker &query);

	// Checks to see if the provided attributed is in this CNF and is sortable.
	virtual bool IsSortableAttribute(const int &attr);

	// print the comparison structure to the screen
	virtual void Print ();

	// virtual std::string ToString (); 
	std::string ToString (const std::string& prefix); 

        // this takes a parse tree for a CNF and converts it into a 2-D
        // matrix storing the same CNF expression.  This function is applicable
        // specifically to the case where there are two relations involved
        virtual void GrowFromParseTree (const struct AndList *parseTree, const Schema *leftSchema,
		const Schema *rightSchema, Record &literal);

        // version of the same function, except that it is used in the case of
        // a relational selection over a single relation so only one schema is used
        virtual void GrowFromParseTree (const struct AndList *parseTree, const Schema *mySchema,
		Record &literal);

};

#endif
