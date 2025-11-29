#ifndef TASK3_BDD_SYMBOLICREACH_H
#define TASK3_BDD_SYMBOLICREACH_H

#include "Task1_PNMLParser.h"
#include <cudd/cuddObj.hh>


#include <map>
#include <vector>
#include <string>
#include <iostream>

class BDDReacher {
public:
    BDDReacher(const Net& net);   // Net được truyền từ Task 1
    ~BDDReacher();

    void computeBDD();           // Tính tập reachable
    BDD getReachableSet();       // Trả về BDD reachable
    void printBDDMarkings();     // In các marking nếu ít

private:
    const Net& net;
    Cudd mgr;            // CUDD manager
    BDD reachableSet;    // Tập reachable cuối cùng

    std::vector<BDD> current_vars;   // trạng thái hiện tại x_i
    std::vector<BDD> next_vars;      // trạng thái kế tiếp x'_i

    // Mapping place.id -> index
    std::map<std::string, int> placeIdToIndex;

    // ========== Các hàm nội bộ ==========
    void initVariables();                         // tạo biến BDD
    BDD buildInitialMarking();                    // M0
    BDD buildSingleTransition(const Transition& t);  // 1 transition
    BDD buildTransitionRelation();                // OR tất cả transition
};

#endif
