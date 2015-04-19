#include "JoinOptimizer.h"

void JoinOptimizer::Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        Statistics& stats,
                        vector<string> &rels,
                        vector<TupleCount> &counts) { }
