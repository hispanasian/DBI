#ifndef JOINOPTIMIZER_H
#define JOINOPTIMIZER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "ParseTree.h"

struct TupleCount {
    double join;
    double select;
};
using namespace std;

class JoinOptimizer {
public:
    void Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        vector<TupleCount> &counts);
};
#endif
