#include "../inc/Task5_OptimizeILP.h"
#include <iostream>
#include <map>
#include <algorithm>

OptimizeILP::OptimizeILP(const Net& net, BDDReacher* bddReacher) 
    : net(net), bddReacher(bddReacher) {
    initDefaultWeights();
}

OptimizeILP::~OptimizeILP() {}

void OptimizeILP::initDefaultWeights() {
    placeWeights.assign(net.places.size(), 1);
}

void OptimizeILP::setWeights(const std::map<std::string, int>& weights) {
    for (size_t i = 0; i < net.places.size(); ++i) {
        if (weights.count(net.places[i].id)) {
            placeWeights[i] = weights.at(net.places[i].id);
        }
    }
}

void OptimizeILP::generateRandomWeights(int minVal, int maxVal) {
    cout << "Generated Random Weights (c vector):\n";
    for (size_t i = 0; i < net.places.size(); ++i) {
        placeWeights[i] = minVal + rand() % (maxVal - minVal + 1);
        cout << "  " << net.places[i].id << ": " << placeWeights[i] << "\n";
    }
}

map<DdNode*, long long> memo;

long long findMaxWeightRecursive(DdManager* dd, DdNode* node, const vector<int>& weights) {
    if (Cudd_IsConstant(node)) {
        if (node == Cudd_ReadOne(dd)) return 0; 
        return -1e18; 
    }

    if (memo.find(node) != memo.end()) {
        return memo[node];
    }

    int index = Cudd_NodeReadIndex(node);
    int place_idx = index / 2;

    DdNode* thenChild = Cudd_T(node);
    DdNode* elseChild = Cudd_E(node);

    long long w_then = findMaxWeightRecursive(dd, thenChild, weights);
    long long w_else = findMaxWeightRecursive(dd, elseChild, weights);

    long long current_w = (place_idx < weights.size()) ? weights[place_idx] : 0;

    if (index % 2 != 0) current_w = 0;

    long long val_then = (w_then > -1e17) ? (current_w + w_then) : -1e18;
    long long val_else = (w_else > -1e17) ? w_else : -1e18;

    long long res = max(val_then, val_else);
    memo[node] = res;
    return res;
}

void recoverOptimalMarking(DdManager* dd, DdNode* node, const vector<int>& weights, vector<int>& marking, map<DdNode*, long long>& memo) {
    if (Cudd_IsConstant(node)) return;

    int index = Cudd_NodeReadIndex(node);
    int place_idx = index / 2;
    
    DdNode* thenChild = Cudd_T(node);
    DdNode* elseChild = Cudd_E(node);

    long long w_then = findMaxWeightRecursive(dd, thenChild, weights);
    long long w_else = findMaxWeightRecursive(dd, elseChild, weights);

    long long current_w = (place_idx < weights.size() && index % 2 == 0) ? weights[place_idx] : 0;

    long long val_then = (w_then > -1e17) ? (current_w + w_then) : -1e18;
    long long val_else = (w_else > -1e17) ? w_else : -1e18;

    if (val_then >= val_else) {
        if (index % 2 == 0 && place_idx < marking.size()) marking[place_idx] = 1;
        recoverOptimalMarking(dd, thenChild, weights, marking, memo);
    } else {
        if (index % 2 == 0 && place_idx < marking.size()) marking[place_idx] = 0;
        recoverOptimalMarking(dd, elseChild, weights, marking, memo);
    }
}
void OptimizeILP::findOptimal() {
    BDD reach = bddReacher->getReachableSet();
    Cudd& mgr = bddReacher->getManager();
    DdManager* dd = mgr.getManager();
    DdNode* node = reach.getNode();

    result.found = false;
    result.maxValue = -1; 

    DdGen *gen;
    int *cube;
    CUDD_VALUE_TYPE value;

    Cudd_ForeachCube(dd, node, gen, cube, value) {
        long long currentWeight = 0;
        Marking currentM;
        currentM.m.assign(net.places.size(), 0);

        for (size_t i = 0; i < net.places.size(); ++i) {
            int bddIndex = i * 2;
  
            if (cube[bddIndex] == 1 || (cube[bddIndex] == 2 && placeWeights[i] > 0)) {
                currentWeight += placeWeights[i];
                currentM.m[i] = 1;
            }
        }

        if (!result.found || currentWeight > result.maxValue) {
            result.maxValue = currentWeight;
            result.optimalMarking = currentM;
            result.found = true;
        }
    }
}
void OptimizeILP::printResult() {
    cout << "\n=== Task 5: Optimization Result (Symbolic DP) ===\n";
    if (result.found) {
        cout << "Max Objective Value (c^T * M): " << result.maxValue << "\n";
        cout << "Optimal Marking: [ ";
        // SỬA LỖI: Duyệt qua .m
        for (int val : result.optimalMarking.m) cout << val << " ";
        cout << "]\n";
    } else {
        cout << "No reachable marking found.\n";
    }
    cout << "============================================================\n";
}

long long OptimizeILP::calculateObjective(const Marking& mk) { return 0; }