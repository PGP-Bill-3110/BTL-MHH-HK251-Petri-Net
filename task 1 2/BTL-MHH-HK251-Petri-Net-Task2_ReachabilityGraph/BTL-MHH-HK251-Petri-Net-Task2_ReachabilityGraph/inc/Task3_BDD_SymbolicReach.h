#ifndef TASK3_BDD_SYMBOLICREACH_H
#define TASK3_BDD_SYMBOLICREACH_H

#include <vector>
#include "cudd/cuddObj.hh"

class BDD_SymbolicReach {
public:
    BDD_SymbolicReach(Cudd& mgr, int numPlaces);
    void encodeMarking(const std::vector<int>& marking);
    void computeReachableSet();
    void printReachableMarkings() const;
    unsigned getNumReachable() const;

private:
    Cudd& mgr;
    int numPlaces;
    std::vector<BDD> placeVars;  // BDD variable for each place
    BDD reachable;               // BDD representing reachable markings
};

#endif
