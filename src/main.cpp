#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"
#include "../inc/Task4_DeadlockILP.h"
#include "../inc/Task5_OptimizeILP.h"
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    // Đảm bảo file pnml nằm đúng đường dẫn test/example_2.pnml so với thư mục chạy lệnh
    string filename = (argc > 1) ? argv[1] : "test/example_2.pnml";

    PNMLParser parser;
    if (!parser.loadFile(filename))
    {
        cout << "Failed to load file: " << filename << endl;
        return 1;
    }

    const vector<Net> &nets = parser.getNets();

    if (nets.empty())
    {
        cout << "No nets found in file!" << endl;
        return 1;
    }

    cout << "Loaded " << nets.size() << " net(s) from file.\n\n";

    for (size_t i = 0; i < nets.size(); ++i)
    {
        cout << "======================================\n";
        cout << "NET #" << i + 1 << "   (ID = " << nets[i].id << ")\n";
        cout << "Places: " << nets[i].places.size()
             << ", Transitions: " << nets[i].transitions.size()
             << ", Arcs: " << nets[i].arcs.size() << "\n";
        cout << "======================================\n";

        // --- TASK 2: Reachability Graph (BFS) ---
        cout << "\n--- [Task 2] Explicit computation (BFS) ---\n";
        auto startBFS = high_resolution_clock::now();

        Graph g(nets[i]);
        cout << "Computing reachable markings...\n";
        g.computeBFS();

        auto endBFS = high_resolution_clock::now();
        duration<double> timeBFS = endBFS - startBFS;
        g.printMarkings();
        cout << ">> BFS Execution Time: " << timeBFS.count() << " seconds.\n";

        // --- TASK 3: Symbolic Reachability (BDD) ---
        cout << "\n--- [Task 3] Symbolic computation (BDD) ---\n";
        auto startBDD = high_resolution_clock::now();

        BDDReacher bddReacher(nets[i]); 
        cout << "Computing reachable markings using BDD...\n";
        bddReacher.computeBDD();

        auto endBDD = high_resolution_clock::now();
        duration<double> timeBDD = endBDD - startBDD;

        bddReacher.printBDDMarkings();
        cout << ">> BDD Execution Time: " << timeBDD.count() << " seconds.\n";

        // --- TASK 4: Deadlock Detection ---
        cout << "\n--- [Task 4] Deadlock Detection ---\n";
        DeadlockILP deadlockFinder(nets[i], &bddReacher);
        cout << "Searching for deadlocks...\n";
        deadlockFinder.findDeadlock();
        deadlockFinder.printResult();

        // --- TASK 5: Optimization ---
        cout << "\n--- [Task 5] Optimization (ILP over BDD) ---\n";
        OptimizeILP optimizer(nets[i], &bddReacher);

        // Random trọng số để test tối ưu hóa
        optimizer.generateRandomWeights(1000, 50000);

        cout << "Searching for optimal marking...\n";
        optimizer.findOptimal();
        optimizer.printResult();

        cout << endl;
    }
    return 0;
}