#include "JoinOptimizer.h"
#include <iostream>

using namespace std;
void JoinOptimizer::Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        Statistics& stats,
                        vector<string> &rels,
                        vector<TupleCount> &counts) {
    // first check if there are any rels to join
    if(selects.size() == 0) {
        // we don't need to do anything
        return;
    }

    // if there is just one relation, we just add it to the output
    if(selects.size() < 3) {
        for(auto it = selects.begin(); it != selects.end(); ++it) {
            rels.push_back(it->first);
            // look up the select size for this relation
            const char* names[] = {it->first.c_str()};
            TupleCount tc {stats.Estimate(it->second, names, 1) , 0};
            counts.push_back(tc);
        } 
        return;
    }
}


