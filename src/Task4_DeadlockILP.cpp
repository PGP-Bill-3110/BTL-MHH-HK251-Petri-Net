#include "../inc/Task4_DeadlockILP.h"

DeadlockILP::DeadlockILP(const Net& net, BDDReacher* bddReacher) 
    : net(net), bddReacher(bddReacher), foundDeadlock(false), checkedCount(0) {
    buildIOMaps();
}

DeadlockILP::~DeadlockILP() {}

void DeadlockILP::buildIOMaps() {
    placeIdToIndex.clear();
    for (size_t i = 0; i < net.places.size(); ++i) {
        placeIdToIndex[net.places[i].id] = i;
    }
    
    inputOf.clear();
    for (const auto& arc : net.arcs) {
        if (placeIdToIndex.find(arc.source) != placeIdToIndex.end()) {
            inputOf[arc.target].push_back(placeIdToIndex[arc.source]);
        }
    }
}

bool DeadlockILP::findDeadlock() {
    BDD reach = bddReacher->getReachableSet();
    Cudd& mgr = bddReacher->getManager();
    
    BDD allDisabled = mgr.bddOne();

    for (const auto& t : net.transitions) {
        BDD t_disabled = mgr.bddZero(); 
        
        if (inputOf.count(t.id)) {
            for (int p_idx : inputOf[t.id]) {
                BDD p_var = bddReacher->getVar(p_idx * 2);  
                t_disabled |= (!p_var); 
            }
        }
        allDisabled &= t_disabled;
    }

    BDD deadlockSet = reach & allDisabled;

    if (deadlockSet.IsZero()) {
        foundDeadlock = false;
        return false;
    }

    foundDeadlock = true;
    cout << "[Found] Deadlock state(s) detected in symbolic space.\n";
    
    deadlockSet.PrintMinterm(); 
    
    return true;
}

void DeadlockILP::printResult() {
    cout << "\n=== Task 4: Deadlock Detection (Symbolic Logic) ===\n";
    if (foundDeadlock) {
        cout << "[RESULT] DEADLOCK FOUND!\n";
    } else {
        cout << "[RESULT] NO DEADLOCK FOUND.\n";
    }
    cout << "========================================\n";
}

bool DeadlockILP::isDeadMarking(const Marking& mk) { return false; }
bool DeadlockILP::isEnabled(const string& tid, const Marking& mk) { return false; }