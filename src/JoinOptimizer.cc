#include "JoinOptimizer.h"
#include <iostream>
#include <limits>
#include <algorithm>


using namespace std;

void Memoizer::SetSoln(vector<bool>& set, double cost, Statistics& stats, int addedIndex, int outputSize) {
    solns.insert(make_pair<vector<bool>&, SolnData*>(set, new SolnData{cost, &stats, addedIndex, outputSize}));
}

double Memoizer::GetCost(vector<bool>& set) {
    return solns.at(set)->cost;
}

Statistics& Memoizer::GetStats(vector<bool>& set) {
    return *(solns.at(set)->stats);
}

int Memoizer::GetAddedIndex(vector<bool>& set) {
    return solns.at(set)->addedIndex;
}

int Memoizer::GetOutputSize(vector<bool>& set) {
    return solns.at(set)->outputSize;
}

bool Memoizer::Solved(vector<bool>& set) {
    auto index = solns.find(set);
    return index != solns.end();
}

JoinOptimizer::~JoinOptimizer() {}

void JoinOptimizer::Optimize(unordered_map<string, AndList*> &selects,
                        unordered_map<string, unordered_map<string, AndList*> > &joins,
                        const Statistics& stats,
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
    cout << "Starting..." << endl;
    Solve(set, relNames, mem, stats, joins);
    cout << "Done solving" << endl;
    // now backtrack to figure out the solution
    // build the rels and count vectors
    cout << "Backtracking..." << endl;
    int index = mem.GetAddedIndex(set);
    while(index != -1) {
        set[index] = false;
        PrintSet(set, relNames);
        // add this relation to our output list
        rels.push_back(relNames[index]);
        counts.push_back(TupleCount{mem.GetOutputSize(set), tupleCount[index]});
        index = mem.GetAddedIndex(set);
    }
    // now add the counts from the first join
    vector<int> indices; 
    Indices(set, indices);
    cout << indices.size() << endl;
    rels.push_back(relNames[indices[0]]);
    counts.push_back(TupleCount{0, tupleCount[indices[0]]});
    rels.push_back(relNames[indices[1]]);
    counts.push_back(TupleCount{0, tupleCount[indices[1]]});

    // reverse our lists
    reverse(rels.begin(), rels.end());
    reverse(counts.begin(), counts.end());
}

void JoinOptimizer::PrintSet(const vector<bool>& set, const vector<string>& relNames) {
    for(int i = 0; i < set.size(); ++i) {
        if(set[i]) {
            cout << relNames[i];
        }
    }
    cout << endl;
}

void JoinOptimizer::Solve(vector<bool>& set,
                            vector<string>& relNames,
                            Memoizer& mem,
                            Statistics& stats,
                            unordered_map<string, unordered_map<string, AndList*> > &joins) {
    cout << "Solving: ";
    PrintSet(set, relNames);
    // base case: we've already calculated this solution
    if(mem.Solved(set)) {
        cout << "Already solved this: ";
        PrintSet(set, relNames);
        return;
    }
    // base case: only 2 relations
    // we just join them directly
    cout << "here 1" << endl;
    vector<int> indices;
    Indices(set, indices);
    if(indices.size() == 2) {
        cout << "here 2" << endl;
        set[indices[0]] = false;
        // get a stats object from one of the relations
        // copy it
        cout << "here 3" << endl;
        Statistics* newStatsPtr = new Statistics(stats);
        Statistics& newStats = *newStatsPtr;
        AndList* andList = GetAndList(indices[0], indices, relNames, joins); 
        if(andList == NULL) {
            cout << "Invalid join " << relNames[indices[0]] << " and " << relNames[indices[1]] << endl;
            // this is an invalid join, set the cost to 
            // be very large so it's not considered a valid solution
            set[indices[0]] = true;
            mem.SetSoln(set, numeric_limits<double>::max(), stats, -1, numeric_limits<double>::max()); 
            return;
        }
        cout << "here 4" << endl;
        // get an array with the names of the relations we are joining together
        const char* names[indices.size()];
        // const char** names = GetRelNames(minIndex, set, relNames);
        GetRelNames(indices[0], indices, names, relNames);
        cout << "here 4" << endl;
        // figure out the cost
        double newCost = newStats.Estimate(andList, names, indices.size());
        cout << "here 5" << endl;
        // Apply and join these 2 relations
        // TODO: this will throw an exception!
        newStats.Apply(andList, names, indices.size());
        cout << "here 6" << endl;
        set[indices[0]] = true;
        // place it in the memoizer
        mem.SetSoln(set, newCost, newStats, -1, newCost);
        cout << "here 7" << endl;
        return;
    } 

    double minCost = numeric_limits<double>::max();
    double minIndex = -1;
    // iterate through every subset
    for(int i = 0; i < indices.size(); ++i) {
       cout << "loop here 1" << endl; 
        int index = indices[i];
        // Check that this join is valid at all
       cout << "loop here 2" << endl; 
        AndList* andList = GetAndList(index, indices, relNames, joins); 
       cout << "loop here 3" << endl; 
        if(andList == NULL) {
            // this is an invalid join, set the cost to 
            // be very large so it's not considered a valid solution
            mem.SetSoln(set, numeric_limits<double>::max(), stats, -1, numeric_limits<double>::max()); 
            continue;
        }
  
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
    cout << "Min index for set "; PrintSet(set, relNames); cout << minIndex << endl; 
    if(minIndex == -1) {
        // we didn't find a solution, this set is impossible
        mem.SetSoln(set, numeric_limits<double>::max(), stats, -1, numeric_limits<double>::max()); 
        return;
    }
    // now we know which relation is optimal to join last
    // compute the soln to this set using the optimal sub-soln
    // store this soln in the mem
    set[minIndex] = false;
    Statistics* oldStatsPtr = &(mem.GetStats(set));
    Statistics& oldStats = *oldStatsPtr;
    cout << "here 7.5" << endl;
    Statistics& newStats = *new Statistics(oldStats);
    cout << "here 8" << endl; 
    // figure out which AndList goes with this join
    AndList* andList = GetAndList(minIndex, indices, relNames, joins); 
    cout << "here 9" << endl; 
    // get an array with the names of the relations we are joining together
    const char* names[indices.size()];
    cout << "here 10" << endl; 
    // const char** names = GetRelNames(minIndex, set, relNames);
    GetRelNames(minIndex, indices, names, relNames);
    cout << "here 11" << endl; 
    // TODO: will throw an exception
    double joinSize = newStats.Estimate(andList, names, indices.size()); 
    cout << "here 12" << endl; 
    double newCost = joinSize + mem.GetCost(set);
    cout << "here 13" << endl; 
    // Apply and join these 2 relations
    newStats.Apply(andList, names, indices.size());
    cout << "here 14" << endl; 
    // store this data in the memoizer
    set[minIndex] = true;
    mem.SetSoln(set, newCost, newStats, minIndex, joinSize);
    cout << "here 15" << endl; 
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
    return NULL;
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

