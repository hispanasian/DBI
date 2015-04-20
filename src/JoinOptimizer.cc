#include "JoinOptimizer.h"

JoinOptimizer::~JoinOptimizer() {}

void JoinOptimizer::Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        const Statistics& stats,
                        vector<string> &rels,
                        vector<TupleCount> &counts) { }
