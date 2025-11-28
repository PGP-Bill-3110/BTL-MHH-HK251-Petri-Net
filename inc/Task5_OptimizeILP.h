#ifndef TASK5_OPTIMIZEILP_H
#define TASK5_OPTIMIZEILP_H

#include "Task1_PNMLParser.h"
#include "Task2_ReachabilityGraph.h"  
#include "Task3_BDD_SymbolicReach.h"
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;

struct OptimizationResult {
    Marking optimalMarking; // Trạng thái tốt nhất tìm được
    long long maxValue;     // Giá trị hàm mục tiêu tương ứng (c^T * M)
    bool found;           
};

class OptimizeILP {
public:
    OptimizeILP(const Net& net, BDDReacher* bddReacher);
    ~OptimizeILP();
    void setWeights(const std::map<std::string, int>& weights);
    void generateRandomWeights(int minVal = 1, int maxVal = 10);
    void findOptimal();
    void printResult();

private:
    const Net& net;
    BDDReacher* bddReacher;
    std::vector<int> placeWeights;
    
    // Lưu kết quả
    OptimizationResult result;
    long long calculateObjective(const Marking& mk);
    void initDefaultWeights();
};

#endif