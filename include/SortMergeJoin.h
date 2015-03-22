#ifndef SORTMERGEJOIN_H
#define SORTMERGEJOIN_H

#include "JoinStrategy.h"
#include "JoinRelation.h"

class SortMergeJoin : public JoinStrategy {
friend class SortMergeJoinTest;
private:
	
	// Aligns tempL and tempR to both be in equal groups, if possible
	// Preconditons:
	// 1. Both pipes are not empty
	// 2. tempL and tempR are set to records from their respective pipes
	// Returns true if at least one of the pipes has been emptied
	// and returns false otherwise
	virtual bool AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp);

	// Initializes relR to contain the entire group matching the group of right
	// Preconditions:
	// 1. groupRec is not null
	// 2. relR is already cleared and reset
	// Postconditions:
	// 1. tempR is set to the record from the next group if there is a next group
	// Returns true if the right relation has been exhausted, and false otherwise
	virtual bool InitRightGroup(Pipe& inPipeR, Record& groupRecR, Record& tempR, JoinRelation& relR,
		OrderMaker& orderR, ComparisonEngine& comp);
public:
	SortMergeJoin();
	~SortMergeJoin();

	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR);
};

#endif /* SORTMERGEJOIN_H */