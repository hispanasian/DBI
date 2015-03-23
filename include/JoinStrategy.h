#ifndef JOINSTRATEGY_H
#define JOINSTRATEGY_H

#include "Pipe.h"
#include "Comparison.h"
#include "Record.h"

class JoinStrategy {
public:
	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe,
		OrderMaker &orderL, OrderMaker &orderR, int memLimit) = 0;
};

#endif /* JOINSTRATEGY_H */