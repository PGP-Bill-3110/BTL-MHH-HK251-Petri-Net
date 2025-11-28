#include "../inc/Task4_DeadlockILP.h"
#include <cmath>

DeadlockILP::DeadlockILP(const Net& inputNet, BDDReacher* reacher) 
    : net(inputNet), bddReacher(reacher), foundDeadlock(false), checkedCount(0) {
    
    // Map place id -> index
    for(int i = 0; i < net.places.size(); i++){
        placeIdToIndex[net.places[i].id] = i;
    }
    
    buildIOMaps();
}

DeadlockILP::~DeadlockILP(){
}

void DeadlockILP::buildIOMaps(){
    inputOf.clear();
    for(const auto& t : net.transitions){
        inputOf[t.id] = {};
    }
    
    for(const auto& arc : net.arcs){
        bool srcIsPlace = false;
        int placeIndex = -1;
        
        for(int i = 0; i < net.places.size(); i++){
            if(net.places[i].id == arc.source){
                srcIsPlace = true;
                placeIndex = i;
                break;
            }
        }
        
        if(srcIsPlace){
            inputOf[arc.target].push_back(placeIndex);
        }
    }
}

bool DeadlockILP::isEnabled(const string& tid, const Marking& mk){
    for(int pIndex : inputOf[tid]){
        if(mk.m[pIndex] != 1){
            return false;
        }
    }
    return true;
}

bool DeadlockILP::isDeadMarking(const Marking& mk){
    for(const auto& t : net.transitions){
        if(isEnabled(t.id, mk)){
            return false; 
        }
    }
    return true; 
}

bool DeadlockILP::findDeadlock(){
    cout << "\n=== Task 4: Deadlock Detection (BDD Enumeration) ===" << endl;
    
    BDD reachSet = bddReacher->getReachableSet();
    
    if(reachSet.IsZero()){
        cout << "[Task4 ERROR] Reachable set is empty. Run computeBDD() first!" << endl;
        return false;
    }
    
    int numPlaces = net.places.size();
    
    // CUDD trả về mảng int (0, 1, 2), KHÔNG PHẢI char
    int* cube; 
    CUDD_VALUE_TYPE value;
    DdGen* gen = Cudd_FirstCube(reachSet.manager(), reachSet.getNode(), &cube, &value);
    
    foundDeadlock = false;
    checkedCount = 0;
    
    if(gen){
        do {
            checkedCount++;
            Marking mk;
            mk.m.resize(numPlaces);
            
            for(int i = 0; i < numPlaces; i++){
                int varIndex = i * 2; // Biến chẵn là trạng thái hiện tại
                // cube[varIndex] == 1 nghĩa là có token
                if(cube[varIndex] == 1){
                    mk.m[i] = 1;
                } else {
                    mk.m[i] = 0;
                }
            }
            
            if(isDeadMarking(mk)){
                foundDeadlock = true;
                deadlockMarking = mk;
                Cudd_GenFree(gen); // Dừng enumerate
                return true;
            }
            
        } while(Cudd_NextCube(gen, &cube, &value) != 0);
        
        Cudd_GenFree(gen);
    }
    
    return false;
}

void DeadlockILP::printResult(){
    cout << "Checked " << checkedCount << " markings." << endl;
    if(foundDeadlock){
        cout << "[RESULT] DEADLOCK FOUND!" << endl;
        cout << "Deadlock Marking: [ ";
        for(int x : deadlockMarking.m) cout << x << " ";
        cout << "]" << endl;
    } else {
        cout << "[RESULT] NO DEADLOCK FOUND." << endl;
    }
    cout << string(40, '=') << endl;
}