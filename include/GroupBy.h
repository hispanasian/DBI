#ifndef INCLUDE_GROUPBY_H_
#define INCLUDE_GROUPBY_H_

#include "RelationalOp.h"
#include "Record.h"
#include "Function.h"
#include "Pipe.h"
#include "ComparisonEngine.h"


class GroupBy : public RelationalOp {

private:
	int pageLimit;
	void computeAndOutputSum(int intSum, double doubleSum, Function& func,
	Record& mergeWith, Record& mergeInto, OrderMaker &groupAtts, int* attsToKeep, Pipe& outPipe);

public:
	GroupBy();
	virtual ~GroupBy();


	/*
		Groups records from inPipe according to the sort order defind in gropuAtts
		computes the sum function on the tuples of each group, and adds a result tuple
		for each group to outPipe.
	 */
    void Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);

	/**
	 * Sets the number of pages to use in the BigQ for soring
	 */
	virtual void Use_n_Pages (int n);

	/**
	 * This method will do the single threaded work for GroupBy
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);

	/**
	 * This method will do the single threaded work for GroupBy
	 */
	virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe,
		Record &rec, Record &prev, Record& mergeInto, ComparisonEngine &comp);

	/*
	 * Returns the pageLimit
	 */
	virtual int GetPageLimit();
};

struct GroupByData {
	Pipe &inPipe;
	Pipe &outPipe;
	OrderMaker &groupAtts;
	Function &computeMe;
	GroupBy &op;
};

#endif /* INCLUDE_GROUPBY_H_ */