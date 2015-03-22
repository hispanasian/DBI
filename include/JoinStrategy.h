#ifndef JOINSTRATEGY_H
#define JOINSTRATEGY_H

#include "Pipe.h"
#include "Comparison.h"
#include "Record.h"

class JoinStrategy {
public:
	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
		Record &literal, OrderMaker &orderL, OrderMaker &orderR) = 0;
};

#endif /* JOINSTRATEGY_H */