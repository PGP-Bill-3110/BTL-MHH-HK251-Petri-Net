#ifndef TASK4_DEADLOCKILP_H
#define TASK4_DEADLOCKILP_H

#include "Task1_PNMLParser.h"
#include "Task2_ReachabilityGraph.h"
#include "Task3_BDD_SymbolicReach.h"
#include <vector>
#include <iostream>

using namespace std;

class DeadlockILP {
public:
    DeadlockILP(const Net& net, BDDReacher* bddReacher);
    ~DeadlockILP();

    // Tìm deadlock bằng enumeration từ BDD
    bool findDeadlock();
    
    // Kiểm tra marking có phải deadlock không
    bool isDeadMarking(const Marking& mk);
    
    // In kết quả deadlock detection
    void printResult();

private:
    const Net& net;
    BDDReacher* bddReacher;
    
    // Lưu deadlock marking nếu tìm thấy
    Marking deadlockMarking;
    bool foundDeadlock;
    
    // Số lượng markings đã check
    int checkedCount;
    
    // Mapping place id -> index
    map<string, int> placeIdToIndex;
    
    // Mapping transition id -> input places indices
    map<string, vector<int>> inputOf;
    
    // Build input/output maps
    void buildIOMaps();
    
    // Check nếu transition enabled tại marking
    bool isEnabled(const string& tid, const Marking& mk);
};

#endif
