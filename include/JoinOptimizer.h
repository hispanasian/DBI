#ifndef JOINOPTIMIZER_H
#define JOINOPTIMIZER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "ParseTree.h"
#include "Statistics.h"

struct TupleCount {
    double join;
    double select;
};
using namespace std;

class JoinOptimizer {
public:
    /*
     * Optimize takes a map from the alias of a relation to the AndList used to
     * select on that relation, as well as a map from 2 relations to the AndList,
     * used to join them, and a Statistics object and calculates the optimal
     * order in which to perform all the joins. This output result is put in the
     * rels and counts output parameters.
     * rels will contain in order the names of the relations which should be joined.
     * rels[0] and rels[1] should first be joined. Then, the result of that join should
     * be joined with rels[2]. The result of that join should be joined with rels[3], and
     * so forth. The list of TupleCounts can be used to determine the proper arrangement
     * of each intermediate result as the left or right operand for each join.
     * counts[i] will give the number of tuples results from the ith join and the number
     * of tuples results from selecting the ith relation. Because rels[0] and rels[1] are
     * the first two relations to be joined, the join count at these indicies will be
     * invalid.
     */
    void Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        Statistics& stats,
                        vector<string> &rels,
                        vector<TupleCount> &counts);
};
#endif

