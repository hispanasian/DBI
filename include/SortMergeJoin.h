#ifndef SORTMERGEJOIN_H
#define SORTMERGEJOIN_H

#include "JoinStrategy.h"

class SortMergeJoin : public JoinStrategy {
public:
	void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR);
};

#endif /* SORTMERGEJOIN_H */