#include "../inc/Task1_PNMLParser.h"
#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"
#include "../inc/Task4_DeadlockILP.h"
#include "../inc/Task5_OptimizeILP.h"

#include <iostream>
#include <iomanip>
#include <chrono> 

using namespace std;

void printSeparator() {
    cout << string(60, '=') << endl;
}

int main() {
    // 1. Đọc file PNML (Mặc định là example_2.pnml để test deadlock)
    string filename = "test/example_2.pnml";
    
    PNMLParser parser;
    if (!parser.loadFile(filename)) {
        cout << "Failed to load file: " << filename << endl;
        return 1;
    }

    const vector<Net> &nets = parser.getNets();
    if (nets.empty()) {
        cout << "No nets found in file!" << endl;
        return 1;
    }

    cout << "Loaded " << nets.size() << " net(s) from file: " << filename << endl;

    // 2. Duyệt qua từng mạng Petri (Net) để chạy 5 Task
    for (size_t i = 0; i < nets.size(); ++i) {
        cout << "\n\n";
        printSeparator();
        cout << "NET #" << i + 1 << "   (ID = " << nets[i].id << ")" << endl;
        cout << "Places: " << nets[i].places.size()
             << ", Transitions: " << nets[i].transitions.size()
             << ", Arcs: " << nets[i].arcs.size() << endl;
        printSeparator();

        // --- TASK 2: Explicit Reachability (BFS) ---
        cout << "\n--- [Task 2] Explicit computation (BFS) ---" << endl;
        auto start2 = chrono::high_resolution_clock::now();
        
        Graph g(nets[i]);
        cout << "Computing reachable markings..." << endl;
        g.computeBFS();
        g.printMarkings();
        
        auto end2 = chrono::high_resolution_clock::now();
        chrono::duration<double> diff2 = end2 - start2;
        cout << ">> BFS Execution Time: " << diff2.count() << " seconds." << endl;


        // --- TASK 3, 4, 5 (Cần dùng chung BDD Manager) ---
        try {
            // --- TASK 3: Symbolic Reachability (BDD) ---
            cout << "\n--- [Task 3] Symbolic computation (BDD) ---" << endl;
            auto start3 = chrono::high_resolution_clock::now();

            // Khởi tạo và tính toán BDD
            BDDReacher bddReacher(nets[i]);
            bddReacher.computeBDD();
            
            auto end3 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff3 = end3 - start3;
            cout << ">> BDD Execution Time: " << diff3.count() << " seconds." << endl;


            // --- TASK 4: Deadlock Detection ---
            cout << "\n--- [Task 4] Deadlock Detection (ILP + BDD) ---" << endl;
            auto start4 = chrono::high_resolution_clock::now();
            cout << "Checking for deadlocks..." << endl;

            DeadlockILP deadlockFinder(nets[i], &bddReacher);
            deadlockFinder.findDeadlock();
            deadlockFinder.printResult();
            
            auto end4 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff4 = end4 - start4;
            cout << ">> Task4 Execution Time: " << diff4.count() << " seconds." << endl;


            // --- TASK 5: Optimization ---
            cout << "\n--- [Task 5] Reachable Optimization ---" << endl;
            auto start5 = chrono::high_resolution_clock::now();
            
            OptimizeILP optimizer(nets[i], &bddReacher);
            optimizer.generateRandomWeights(-5, 10); // Random từ -5 đến 10
            
            optimizer.findOptimal();
            optimizer.printResult();
            
            auto end5 = chrono::high_resolution_clock::now();
            chrono::duration<double> diff5 = end5 - start5;
            cout << ">> Task5 Execution Time: " << diff5.count() << " seconds." << endl;

        } catch (const std::exception &e) {
            cerr << "[CRITICAL ERROR] Exception in Symbolic Tasks: " << e.what() << endl;
        }
    }

    return 0;
}