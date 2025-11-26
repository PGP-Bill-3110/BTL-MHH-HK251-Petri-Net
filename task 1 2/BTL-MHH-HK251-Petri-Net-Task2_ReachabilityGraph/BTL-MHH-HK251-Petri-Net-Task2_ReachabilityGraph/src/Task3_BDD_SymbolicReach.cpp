#include "../inc/Task3_BDD_SymbolicReach.h"
#include  <cmath>


BDDReacher::BDDReacher(const Net& inputNet) : net(inputNet), mgr(0, 0){
    for(int i = 0; i < net.places.size(); i ++){
        placeIdToIndex[net.places[i].id] = i;
    }
    initVariables();
}
BDDReacher::~BDDReacher(){
    current_vars.clear();
    next_vars.clear();
}

void BDDReacher::initVariables(){
    int numPlaces = net.places.size();

    for(int i = 0; i < numPlaces; i++){
        current_vars.push_back(mgr.bddVar(i * 2));//even for current state
        next_vars.push_back(mgr.bddVar(i * 2 + 1));//odd for next state
    }
}

BDD BDDReacher::buildInitialMarking(){
    BDD M0 = mgr.bddOne();

    for(int i = 0; i < net.places.size(); i++){
        //and place has tokens != 0
        if(net.places[i].tokens != 0){
            M0 = M0 * current_vars[i];
        }
        else M0 = M0 * (!current_vars[i]);
    }
    return M0;
}
BDD BDDReacher::buildSingleTransition(const Transition& t){
    vector<int> inputs, outputs;

    for(const auto& arc : net.arcs){
        //arc: transition -> place => output
        if(arc.source == t.id){
            if(placeIdToIndex.count(arc.target)){
                outputs.push_back(placeIdToIndex[arc.target]);
            }
        }
        //arc: places -> transition => input
        else if(arc.target == t.id){
            if(placeIdToIndex.count(arc.source)){
                inputs.push_back(placeIdToIndex[arc.source]);
            }
        }
    }

    //pre condition
    BDD pre = mgr.bddOne();
    for(int index : inputs){
        pre = pre * current_vars[index];
    }

    //post condition - frame condition
    BDD post = mgr.bddOne();
    vector<bool> isAffected(net.places.size(), false);
    
    //input lost token -> next state = 0
    for(int index : inputs){
        isAffected[index] = true;

        //except self loop
        bool isSelfLoop = false;
        for(int outIndex : outputs){
            if(outIndex == index) isSelfLoop = true;
        }

        if(!isSelfLoop){
            post = post * (!next_vars[index]);
        }
    }
    //output get token -> next_state = 1
    for(int index : outputs){
        isAffected[index] = true;
        post = post * next_vars[index];
    }
    //frame condition: place not affected -> use Xnor
    for(int i = 0; i < net.places.size(); i++){
        if(!isAffected[i]){
            post = post * current_vars[i].Xnor(next_vars[i]);
        }
    }

    return pre * post;
}

BDD BDDReacher::buildTransitionRelation(){
    BDD TR = mgr.bddZero();

    // TR all = or of singleTR
    for(const auto& t : net.transitions){
        TR = TR + buildSingleTransition(t);
    }
    return TR;
}

void BDDReacher::computeBDD(){
    BDD M0 = buildInitialMarking();
    BDD TR = buildTransitionRelation();

    BDD R_current = M0;
    BDD R_prev = mgr.bddZero();

    int step = 0;

    //fixpoint iteration
    while(R_current != R_prev){
        step++;
        R_prev = R_current;

        //syntax: next = Exist(x)[current(x) AND tr(x, x')]
        //create cube contain all current state
        BDD cube = mgr.bddOne();
        for(auto var : current_vars){
            cube = cube * var;
        }

        // AND TR, abstract x variavble
        BDD next_prime = R_current.AndAbstract(TR, cube);
        //swap  x' to x
        BDD next = next_prime.SwapVariables(next_vars, current_vars);

        //R_new = R_current OR next
        R_current = R_current + next;
    }
    double totalMinterms = R_current.CountMinterm(2 * net.places.size());
    double totalStates = totalMinterms / pow(2, net.places.size());
    
    cout << "Converged after " << step << " steps." << endl;
    cout << "Total Reachable Markings: " << (long long)totalStates << endl;
    cout << "BDD Nodes used: " << R_current.nodeCount() << endl;
    cout << "=====================================================" << endl;
}
