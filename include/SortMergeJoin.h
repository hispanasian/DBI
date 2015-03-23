#ifndef SORTMERGEJOIN_H
#define SORTMERGEJOIN_H

#include "JoinStrategy.h"
#include "JoinRelation.h"
#include "InMemoryRelation.h"

class SortMergeJoin : public JoinStrategy {
friend class SortMergeJoinTest;
friend class PartialSortMergeJoin;
private:
	
	// Aligns tempL and tempR to both be in equal groups, if possible
	// Preconditons:
	// 1. tempL and tempR are set to records from their respective pipes
	// Postconditions:
	// 1. tempL and tempR are set to first of records from the left and right
	//    which are in the same group
	// Returns true if at least one of the relations has been exhausted
	// and returns false otherwise
	virtual bool AlignGroups(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp);

	// Initializes relR to contain the entire group matching the group of right
	// Preconditions:
	// 1. groupRec is not null
	// 2. relR is already cleared and reset
	// 3. A COPY of groupRecR has been added to relR
	// Postconditions:
	// 1. tempR is set to the record from the next group if there is a next group
	// Returns true if the right relation has been exhausted, and false otherwise
	virtual bool InitRightGroup(Pipe& inPipeR, Record& groupRecR, Record& tempR, JoinRelation& relR,
		OrderMaker& orderR, ComparisonEngine& comp);

	// Reads the current group from left relation, buffering as much as possible in memory
	// before merging the buffer with right relation
	// Preconditions:
	// 1. relL has been cleared and reset
	// 2. groupRecL is properly initialized
	// 3. a COPY of groupRecL has already been added to relL
	// 4. relR has been filled
	// Postconditions:
	// 1. tempL is set to the record from the next group if there is a next group
	// Returns true if the left relation has been exhausted, and false otherwise
	virtual bool StreamLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp);

	// Creates a merged tuple for each pair of tuples from relL and relR and inserts
	// each merged tuple in outPipe
	virtual void MergeRelations(InMemoryRelation& relL, JoinRelation& relR, Pipe& outPipe, Record& rec);
public:
	SortMergeJoin();
	~SortMergeJoin();

	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR);
};

#endif /* SORTMERGEJOIN_H */