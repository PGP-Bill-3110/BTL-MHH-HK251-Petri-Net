#ifndef TASK3_BDD_SYMBOLICREACH_H
#define TASK3_BDD_SYMBOLICREACH_H

#include "Task1_PNMLParser.h"

#include "cuddObj.hh" // CUDD C++ Wrapper library
#include <map>
#include <vector>
#include <string>

class BDDReacher{
public:
    BDDReacher(const Net& net); //Net[id, source, targer, weight]
    ~BDDReacher();

    void computeBDD();
    BDD getReachableSet();
    void printBDDMarkings();
private:
    const Net& net;
    Cudd mgr; //cudd manager
    BDD reachableSet;
    

    std::vector<    BDD> current_vars;  //current state of x_i
    std::vector<BDD> next_vars;     //next state of x_i

    //Mapping string place.id to index
    std::map<std::string, int> placeIdToIndex;
    //init BDD variables
    void initVariables();
    //build BDD for init state - M_0
    BDD buildInitialMarking();
    //build BDD for ONE transition relate 
    BDD buildSingleTransition(const Transition& t);
    //build BDD for all transition ralate
    BDD buildTransitionRelation();
};


#endif