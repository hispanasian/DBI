#ifndef SORTMERGEJOIN_H
#define SORTMERGEJOIN_H

#include "JoinStrategy.h"

class SortMergeJoin : public JoinStrategy {
friend class SortMergeJoinTest;
private:
	
	// Preconditons:
	// 1. Both pipes are not empty
	// 2. tempL and tempR are set to records from their respective pipes
	// Returns true if at least one of the pipes has been emptied
	// and returns false otherwise
	virtual bool AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp);
public:
	SortMergeJoin();
	~SortMergeJoin();

	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR);
};

#endif /* SORTMERGEJOIN_H */