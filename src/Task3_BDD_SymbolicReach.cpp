#include "../inc/Task3_BDD_SymbolicReach.h"
#include <cmath>
#include <iostream>

using namespace std;

BDDReacher::BDDReacher(const Net& inputNet) 
    : net(inputNet), mgr(0, 0), reachableSet(mgr.bddZero()) {
    for (int i = 0; i < net.places.size(); i++) {
        placeIdToIndex[net.places[i].id] = i;
    }
    initVariables();
}

BDDReacher::~BDDReacher() {
    current_vars.clear();
    next_vars.clear();
}

void BDDReacher::initVariables() {
    int numPlaces = net.places.size();
    for (int i = 0; i < numPlaces; i++) {
        current_vars.push_back(mgr.bddVar(i * 2));     // even for current state
        next_vars.push_back(mgr.bddVar(i * 2 + 1));    // odd for next state
    }
}

BDD BDDReacher::buildInitialMarking() {
    BDD M0 = mgr.bddOne();
    for (int i = 0; i < net.places.size(); i++) {
        if (net.places[i].tokens != 0)
            M0 = M0 * current_vars[i];
        else
            M0 = M0 * (!current_vars[i]);
    }
    return M0;
}

BDD BDDReacher::buildSingleTransition(const Transition& t) {
    vector<int> inputs, outputs;

    // Phân loại input / output của transition
    for (const auto& arc : net.arcs) {
        if (arc.source == t.id) {        // transition -> place => output
            if (placeIdToIndex.count(arc.target))
                outputs.push_back(placeIdToIndex[arc.target]);
        } else if (arc.target == t.id) { // place -> transition => input
            if (placeIdToIndex.count(arc.source))
                inputs.push_back(placeIdToIndex[arc.source]);
        }
    }

    if (inputs.empty() && outputs.empty()) {
        cout << "[BDD ERROR] Transition " << t.id << " has no input or output." << endl;
        return mgr.bddZero();
    }

    // pre-condition: tất cả input = 1
    BDD pre = mgr.bddOne();
    for (int idx : inputs)
        pre = pre * current_vars[idx];

    // post-condition & frame condition
    BDD post = mgr.bddOne();
    vector<bool> isAffected(net.places.size(), false);

    // Input mất token -> next = 0 (trừ self-loop)
    for (int idx : inputs) {
        isAffected[idx] = true;
        bool selfLoop = false;
        for (int outIdx : outputs) {
            if (outIdx == idx) selfLoop = true;
        }
        if (!selfLoop) post = post * (!next_vars[idx]);
    }

    // Output nhận token -> next = 1
    for (int idx : outputs) {
        isAffected[idx] = true;
        post = post * next_vars[idx];
    }

    // Frame condition: những place không bị ảnh hưởng -> giữ nguyên
    for (int i = 0; i < net.places.size(); i++) {
        if (!isAffected[i]) post = post * current_vars[i].Xnor(next_vars[i]);
    }

    return pre * post;
}

BDD BDDReacher::buildTransitionRelation() {
    BDD TR = mgr.bddZero();
    for (const auto& t : net.transitions) {
        TR = TR + buildSingleTransition(t);
    }
    return TR;
}

BDD BDDReacher::getReachableSet() {
    return reachableSet;
}

void BDDReacher::computeBDD() {
    BDD M0 = buildInitialMarking();
    BDD TR = buildTransitionRelation();

    BDD R_current = M0;
    BDD R_prev = mgr.bddZero();
    int step = 0;

    while (R_current != R_prev) {
        step++;
        R_prev = R_current;

        // Exist abstraction: next = ∃x[current(x) AND TR(x,x')]
        BDD cube = mgr.bddOne();
        for (auto var : current_vars)
            cube = cube * var;

        BDD next_prime = R_current.AndAbstract(TR, cube);
        BDD next = next_prime.SwapVariables(next_vars, current_vars);

        R_current = R_current + next;
    }

    reachableSet = R_current;

    double totalMinterms = R_current.CountMinterm(2 * net.places.size());
    double totalStates = totalMinterms / pow(2, net.places.size());

    cout << "Converged after " << step << " steps." << endl;
    cout << "Total Reachable Markings: " << (long long)totalStates << endl;
    cout << "BDD Nodes used: " << R_current.nodeCount() << endl;
}

void BDDReacher::printBDDMarkings() {
    if (reachableSet.IsZero()) {
        cout << "[BDD] No reachable markings found (or not computed yet)." << endl;
        return;
    }

    double numStates = reachableSet.CountMinterm(2 * net.places.size()) / pow(2, net.places.size());
    if (numStates > 20) {
        cout << "[BDD] Too many states (" << numStates << ") to print individually." << endl;
        return;
    }

    cout << "[BDD] Reachable Markings (Minterms):" << endl;
    cout << "Format: Variable indices (0,2,4,... corresponds to Places p0,p1,p2,...)" << endl;
    reachableSet.PrintMinterm();
}
