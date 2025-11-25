#include "Task3_BDD_SymbolicReach.h"
#include <iostream>

BDD_SymbolicReach::BDD_SymbolicReach(Cudd& mgr_, int numPlaces_)
    : mgr(mgr_), numPlaces(numPlaces_), reachable(mgr.bddZero())
{
    placeVars.resize(numPlaces);
    for (int i = 0; i < numPlaces; i++) {
        placeVars[i] = mgr.bddVar(i);
    }
}

// Encode a concrete marking into BDD
void BDD_SymbolicReach::encodeMarking(const std::vector<int>& marking) {
    BDD bddMark = mgr.bddOne();
    for (int i = 0; i < numPlaces; i++) {
        if (marking[i])
            bddMark &= placeVars[i];
        else
            bddMark &= !placeVars[i];
    }
    reachable |= bddMark;
}

// Compute reachable set symbolically (simplified)
void BDD_SymbolicReach::computeReachableSet() {
    // TODO: symbolic image computation with transitions
    // For now just print current reachable
}

// Print reachable markings (can enumerate for small models)
void BDD_SymbolicReach::printReachableMarkings() const {
    std::cout << "BDD size: " << reachable.nodeCount() << " nodes" << std::endl;
}

// Return number of reachable markings
unsigned BDD_SymbolicReach::getNumReachable() const {
    if (reachable.IsZero()) return 0;
    return reachable.CountMinterm(numPlaces);  // numPlaces = số place trong net
}

