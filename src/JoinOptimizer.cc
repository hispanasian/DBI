#include "JoinOptimizer.h"
#include <iostream>
#include <limits>


using namespace std;

void Memoizer::SetSoln(vector<bool>& set, double cost, Statistics& stats, vector<bool>& prevSet) {
    solns.emplace(set, SolnData{cost, stats, prevSet});
}

double Memoizer::GetCost(vector<bool>& set) {
    return solns.at(set).cost;
}

Statistics& Memoizer::GetStats(vector<bool>& set) {
    return solns.at(set).stats;
}

vector<bool>& Memoizer::GetPrev(vector<bool>& set) {
    return solns.at(set).prevSet;
}

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

    // we're need to actually recursively solve this input
    Memoizer mem = Memoizer();
    // TODO
    // set up the solns to the individual relations
    // set the stats objects using the select ands
    // create the set object
    vector<bool> set;
    vector<string> relNames;
    int index = 0;
    for(auto it = selects.begin(); it != selects.end(); ++it) {
        relNames.push_back(it->first);
        set.push_back(true);
        ++index;
    }
    Solve(set, relNames, mem, joins);
    // now backtrack to figure out the solution
    // build the rels and count vectors

}

void JoinOptimizer::Solve(vector<bool>& set, vector<string>& relNames, Memoizer& mem,
                        unordered_map<string, unordered_map<string, AndList*> > &joins) {
    // base case: only 2 relations
    // we just join them directly
    vector<int> indices;
    Indices(set, indices);
    if(indices.size() == 2) {
        // TODO
        // get a stats object from one of the relations
        // copy it
        // Apply and join these 2 relations
        // place it in the memoizer
        return;
    } 

    double minCost = numeric_limits<double>::max();
    double minIndex = -1;
    // iterate through every subset
    for(int i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        // here we'll find the optimal soln assuming that rel index
        // is the last relation to be joined
        // take this rel out of the set
        set[index] = false;
        Solve(set, relNames, mem, joins);
        // check if this solution is better than our current min
        if(minCost > mem.GetCost(set)) {
            minCost = mem.GetCost(set);
            minIndex = index;
        }
        // restore this relation into the set
        set[index] = true;
    }
 
    // now we know which relation is optimal to join last
    // compute the soln to this set using the optimal sub-soln
    // store this soln in the mem
    set[minIndex] = false;
    Statistics newStats = Statistics(mem.GetStats(set));
    // figure out which AndList goes with this join
    AndList* andList = GetAndList(minIndex, indices, relNames, joins); 
    // get an array with the names of the relations we are joining together
    const char** names = GetRelNames(minIndex, set, relNames);
    double newCost = newStats.Estimate(andList, names, indices.size());
    newStats.Apply(andList, names, indices.size());
    // store this data in the memoizer
    vector<bool> prevSet = set;
    set[minIndex] = true;
    mem.SetSoln(set, newCost, newStats, prevSet);
}

AndList* JoinOptimizer::GetAndList(const int index, const vector<int>& indices,
                        const vector<string>& relNames,
                        const unordered_map<string, unordered_map<string, AndList*> > &joins) {
    auto minJoins = joins.at(relNames[index]);
    for(int i = 0; i < indices.size(); ++i) {
        auto contains = minJoins.find(relNames[indices[i]]);
        if(contains != minJoins.end()) {
            return contains->second;
        }
    }
    throw std::runtime_error("Could not find AndList for join "+relNames[index]);
}

const char** JoinOptimizer::GetRelNames(const int index, const vector<bool>& set,
                        const vector<string>& relNames) {
    return NULL; 
    //TODO
}

void JoinOptimizer::Indices(const vector<bool>& set, vector<int>& indices) {
    for(int i = 0; i < set.size(); ++i) {
        if(set[i]) indices.push_back(i);
    }
}

bool JoinOptimizer::SetsEqual(const vector<bool>& set1, const vector<bool>& set2) {
    if(set1.size() == set2.size()) {
        for(int i = 0; i < set1.size(); ++i) {
            if(set1[i] != set2[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

