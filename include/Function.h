
#ifndef INCLUDE_FUNCTION_H_
#define INCLUDE_FUNCTION_H_

#include "Record.h"
#include "ParseFunc.h"

#define MAX_DEPTH 100


enum ArithOp {PushInt, PushDouble, ToDouble, ToDouble2Down, 
	IntUnaryMinus, IntMinus, IntPlus, IntDivide, IntMultiply,
	DblUnaryMinus, DblMinus, DblPlus, DblDivide, DblMultiply};

struct Arithmatic {

	ArithOp myOp;
	int recInput;
	void *litInput;	
};

class Function {
friend class SumTest;
friend class GroupByTest;
private:

	Arithmatic *opList;
	int numOps;

	int returnsInt;

public:

	Function ();
	virtual ~Function();

	// this grows the specified function from a parse tree and converts
	// it into an accumulator-based computation over the attributes in
	// a record with the given schema; the record "literal" is produced
	// by the GrowFromParseTree method
	virtual void GrowFromParseTree (struct FuncOperator *parseTree, Schema &mySchema);

	// helper function
	virtual Type RecursivelyBuild (struct FuncOperator *parseTree, Schema &mySchema);

	// prints out the function to the screen
	virtual void Print ();

	// applies the function to the given record and returns the result
	virtual Type Apply (Record &toMe, int &intResult, double &doubleResult);

	// Returns true if this returns an int
	virtual bool ReturnsInt();
};

#endif
