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
    // Cleanup nếu cần
}

void DeadlockILP::buildIOMaps(){
    inputOf.clear();
    
    // Khởi tạo empty vectors cho mỗi transition
    for(const auto& t : net.transitions){
        inputOf[t.id] = {};
    }
    
    // Build input map từ arcs
    for(const auto& arc : net.arcs){
        // Kiểm tra nếu source là place (place -> transition)
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
            // arc: place -> transition => input
            inputOf[arc.target].push_back(placeIndex);
        }
    }
}

bool DeadlockILP::isEnabled(const string& tid, const Marking& mk){
    // Kiểm tra tất cả input places có token = 1
    for(int pIndex : inputOf[tid]){
        if(mk.m[pIndex] != 1){
            return false;
        }
    }
    return true;
}

bool DeadlockILP::isDeadMarking(const Marking& mk){
    // Dead marking = không có transition nào enabled
    for(const auto& t : net.transitions){
        if(isEnabled(t.id, mk)){
            return false; // Có ít nhất 1 transition enabled
        }
    }
    return true; // Không có transition nào enabled -> deadlock
}

bool DeadlockILP::findDeadlock(){
    cout << "\n=== Task 4: Deadlock Detection ===" << endl;
    cout << "Enumerating reachable markings from BDD..." << endl;
    
    // Lấy BDD reachable set từ Task3
    BDD reachSet = bddReacher->getReachableSet();
    
    if(reachSet.IsZero()){
        cout << "[Task4 ERROR] Reachable set is empty. Run computeBDD() first!" << endl;
        return false;
    }
    
    // Tính số markings để enumerate
    int numPlaces = net.places.size();
    double totalMinterms = reachSet.CountMinterm(2 * numPlaces);
    double totalStates = totalMinterms / pow(2, numPlaces);
    
    cout << "Total reachable markings: " << (long long)totalStates << endl;
    
    // Enumerate từng marking từ BDD
    cout << "Checking for deadlocks..." << endl;
    
    // Sử dụng AllSat để enumerate
    // Format: biến BDD ở vị trí chẵn (0,2,4...) tương ứng places (p0,p1,p2...)
    vector<int> current_state(numPlaces);
    
    // Đơn giản: enumerate bằng cách duyệt tất cả satisfying assignments
    // CUDD C++ API không có built-in enumerate, phải tự implement
    
    // Để đơn giản, dùng approach: duyệt tất cả minterms
    char* minterm = new char[2 * numPlaces + 1];
    DdGen* gen = Cudd_FirstCube(reachSet.manager(), reachSet.getNode(), &minterm, nullptr);
    
    foundDeadlock = false;
    checkedCount = 0;
    
    if(gen){
        do {
            checkedCount++;
            
            // Extract marking từ minterm
            // Chỉ lấy biến chẵn (current state): 0, 2, 4, ...
            Marking mk;
            mk.m.resize(numPlaces);
            
            for(int i = 0; i < numPlaces; i++){
                int varIndex = i * 2; // Biến chẵn
                if(minterm[varIndex] == '1'){
                    mk.m[i] = 1;
                } else {
                    mk.m[i] = 0;
                }
            }
            
            // Kiểm tra deadlock
            if(isDeadMarking(mk)){
                foundDeadlock = true;
                deadlockMarking = mk;
                
                cout << "\n*** DEADLOCK FOUND! ***" << endl;
                cout << "Deadlock marking: [ ";
                for(int x : mk.m) cout << x << " ";
                cout << "]" << endl;
                cout << "Checked " << checkedCount << " markings before finding deadlock." << endl;
                
                Cudd_GenFree(gen);
                delete[] minterm;
                return true;
            }
            
            // Progress indicator
            if(checkedCount % 100 == 0){
                cout << "  Checked " << checkedCount << " markings..." << endl;
            }
            
        } while(Cudd_NextCube(gen, &minterm, nullptr) != 0);
        
        Cudd_GenFree(gen);
    }
    
    delete[] minterm;
    
    cout << "\nNo deadlock found." << endl;
    cout << "Checked all " << checkedCount << " markings." << endl;
    
    return false;
}

void DeadlockILP::printResult(){
    cout << "\n" << string(60, '=') << endl;
    cout << "TASK 4: DEADLOCK DETECTION RESULT" << endl;
    cout << string(60, '=') << endl;
    
    if(foundDeadlock){
        cout << "Status: DEADLOCK EXISTS" << endl;
        cout << string(60, '-') << endl;
        
        cout << "\nDeadlock Marking: [ ";
        for(int x : deadlockMarking.m) cout << x << " ";
        cout << "]" << endl;
        
        cout << "\nPlace details:" << endl;
        for(int i = 0; i < net.places.size(); i++){
            cout << "  " << net.places[i].name 
                 << " (" << net.places[i].id << "): " 
                 << deadlockMarking.m[i] << " token(s)" << endl;
        }
        
        cout << "\nEnabled transitions at deadlock: NONE" << endl;
        
    } else {
        cout << "Status: NO DEADLOCK" << endl;
        cout << string(60, '-') << endl;
        cout << "\nThe Petri net is deadlock-free!" << endl;
        cout << "All " << checkedCount << " reachable markings have at least one enabled transition." << endl;
    }
    
    cout << string(60, '=') << endl;
}
