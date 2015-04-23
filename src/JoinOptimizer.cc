#include "JoinOptimizer.h"
#include <iostream>
#include <limits>
#include <algorithm>


using namespace std;

void Memoizer::SetSoln(vector<bool>& set, double cost, Statistics& stats, int addedIndex, int outputSize) {
    solns.emplace(set, SolnData{cost, stats, addedIndex, outputSize});
}

double Memoizer::GetCost(vector<bool>& set) {
    return solns.at(set).cost;
}

Statistics& Memoizer::GetStats(vector<bool>& set) {
    return solns.at(set).stats;
}

int Memoizer::GetAddedIndex(vector<bool>& set) {
    return solns.at(set).addedIndex;
}

int Memoizer::GetOutputSize(vector<bool>& set) {
    return solns.at(set).outputSize;
}

bool Memoizer::Solved(vector<bool>& set) {
    auto index = solns.find(set);
    return index != solns.end();
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
    // create the set object
    vector<bool> set;
    vector<string> relNames;
    vector<int> tupleCount;
    const char* names[1];
    for(auto it = selects.begin(); it != selects.end(); ++it) {
        // set up the solns to the individual relations
        relNames.push_back(it->first);
        set.push_back(true);
        names[0] = it->first.c_str();
        tupleCount.push_back(stats.Estimate(it->second, names, 1));
        stats.Apply(it->second, names, 1);
    }

    // Start the recursion
    Solve(set, relNames, mem, stats, joins);
    // now backtrack to figure out the solution
    // build the rels and count vectors

    int index = mem.GetAddedIndex(set);
    while(index != -1) {
        set[index] = false;
        // add this relation to our output list
        rels.push_back(relNames[index]);
        counts.push_back(TupleCount{mem.GetOutputSize(set), tupleCount[index]});
        index = mem.GetAddedIndex(set);
    }

    // reverse our lists
    reverse(rels.begin(), rels.end());
    reverse(counts.begin(), counts.end());
}

void JoinOptimizer::Solve(vector<bool>& set,
                            vector<string>& relNames,
                            Memoizer& mem,
                            Statistics& stats,
                            unordered_map<string,
                            unordered_map<string,
                            AndList*> > &joins) {
    // base case: we've already calculated this solution
    if(mem.Solved(set)) {
        return;
    }
    // base case: only 2 relations
    // we just join them directly
    vector<int> indices;
    Indices(set, indices);
    if(indices.size() == 2) {
        set[indices[0]] = false;
        // get a stats object from one of the relations
        // copy it
        Statistics newStats = Statistics(stats);
        AndList* andList = GetAndList(indices[0], indices, relNames, joins); 
        // get an array with the names of the relations we are joining together
        const char* names[indices.size()];
        // const char** names = GetRelNames(minIndex, set, relNames);
        GetRelNames(indices[0], indices, names, relNames);
        // figure out the cost
        double newCost = newStats.Estimate(andList, names, indices.size());
        // Apply and join these 2 relations
        // TODO: this will throw an exception!
        newStats.Apply(andList, names, indices.size());
        vector<bool> prevSet = set;
        set[indices[0]] = true;
        // place it in the memoizer
        mem.SetSoln(set, newCost, newStats, -1, newCost);
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
        Solve(set, relNames, mem, stats, joins);
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
    const char* names[indices.size()];
    // const char** names = GetRelNames(minIndex, set, relNames);
    GetRelNames(minIndex, indices, names, relNames);
    // TODO: will throw an exception
    double joinSize = newStats.Estimate(andList, names, indices.size()); 
    double newCost = joinSize + mem.GetCost(set);
        // Apply and join these 2 relations
        newStats.Apply(andList, names, indices.size());
    newStats.Apply(andList, names, indices.size());
    // store this data in the memoizer
    set[minIndex] = true;
    mem.SetSoln(set, newCost, newStats, minIndex, joinSize);
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

const void JoinOptimizer::GetRelNames(const int index, const vector<int>& indices, const char* names[],
                        const vector<string>& relNames) {
    names[0] = relNames[index].c_str();
    int arrIndex = 1;
    for(int i = 0; i < indices.size(); ++i) {
        if(indices[i] != index) {
           names[arrIndex++] = relNames[indices[i]].c_str(); 
        }
    }
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

