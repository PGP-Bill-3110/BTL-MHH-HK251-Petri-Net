#include "../inc/Task5_OptimizeILP.h"
#include "Task1_PNMLParser.h"
#include "Task3_BDD_SymbolicReach.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits> 

OptimizeILP::OptimizeILP(const Net& inputNet, BDDReacher* reacher) 
    : net(inputNet), bddReacher(reacher) {
    
    // Khởi tạo trọng số mặc định ban đầu là 1
    initDefaultWeights();
    
    // Reset kết quả
    result.found = false;
    result.maxValue = std::numeric_limits<long long>::min();
}

OptimizeILP::~OptimizeILP() {}

void OptimizeILP::initDefaultWeights() {
    placeWeights.assign(net.places.size(), 1);
}

void OptimizeILP::setWeights(const std::map<std::string, int>& weights) {
    for (size_t i = 0; i < net.places.size(); ++i) {
        string id = net.places[i].id;
        if (weights.count(id)) {
            placeWeights[i] = weights.at(id);
        } else {
            placeWeights[i] = 0; 
        }
    }
}

void OptimizeILP::generateRandomWeights(int minVal, int maxVal) {
    srand(time(NULL)); 
    
    cout << "Generated Random Weights (c vector):" << endl;
    for (size_t i = 0; i < net.places.size(); ++i) {
        placeWeights[i] = minVal + rand() % (maxVal - minVal + 1);
        
        cout << "  " << net.places[i].id << ": " << placeWeights[i] << endl;
    }
}

long long OptimizeILP::calculateObjective(const Marking& mk) {
    long long total = 0;
    for (size_t i = 0; i < mk.m.size(); ++i) {
        if (mk.m[i] == 1) {
            total += placeWeights[i];
        }
    }
    return total;
}

void OptimizeILP::findOptimal() {
    cout << "\n=== Task 5: Optimization over Reachable Markings ===" << endl;

    BDD reachSet = bddReacher->getReachableSet();
    
    if (reachSet.IsZero()) {
        cout << "[Error] Reachable set is empty. Run Task 3 first!" << endl;
        result.found = false;
        return;
    }

    int numPlaces = net.places.size();
    int* cube; 
    CUDD_VALUE_TYPE value;
    DdGen* gen = Cudd_FirstCube(reachSet.manager(), reachSet.getNode(), &cube, &value);
    
    result.maxValue = std::numeric_limits<long long>::min();
    result.found = false;
    int checkedCubes = 0;

    if (gen) {
        do {
            checkedCubes++;
            Marking currentMarking;
            currentMarking.m.resize(numPlaces);
            long long currentScore = 0;

            for (int i = 0; i < numPlaces; i++) {
                int varIndex = i * 2; 
                int val = cube[varIndex]; 
                if (val == 1) {
                    currentMarking.m[i] = 1;
                    currentScore += placeWeights[i];
                } 
                else if (val == 0) {               
                    currentMarking.m[i] = 0;
                } 
                else if (val == 2) {
                    
                    if (placeWeights[i] > 0) {
                        currentMarking.m[i] = 1;
                        currentScore += placeWeights[i];
                    } else {
                        currentMarking.m[i] = 0;
                    }
                }
            }

            if (!result.found || currentScore > result.maxValue) {
                result.maxValue = currentScore;
                result.optimalMarking = currentMarking;
                result.found = true;
            }

        } while (Cudd_NextCube(gen, &cube, &value) != 0);
        
        Cudd_GenFree(gen);
    }
    
    cout << "Checked " << checkedCubes << " disjoint cubes from BDD." << endl;
}

void OptimizeILP::printResult() {
    cout << "\n" << string(60, '=') << endl;
    cout << "TASK 5: OPTIMIZATION RESULT" << endl;
    cout << string(60, '=') << endl;
    
    if (result.found) {
        cout << "Max Objective Value (c^T * M): " << result.maxValue << endl;
        cout << "Optimal Marking: [ ";
        for (int x : result.optimalMarking.m) cout << x << " ";
        cout << "]" << endl;
        
        // In chi tiết place nào đóng góp vào kết quả
        cout << "\nContribution details:" << endl;
        for(size_t i=0; i<net.places.size(); ++i) {
            if(result.optimalMarking.m[i] == 1) {
                cout << "  + " << net.places[i].id << " (weight=" << placeWeights[i] << ")" << endl;
            }
        }
        
    } else {
        cout << "Status: NO SOLUTION FOUND (Reachable set might be empty)" << endl;
    }
    cout << string(60, '=') << endl;
}