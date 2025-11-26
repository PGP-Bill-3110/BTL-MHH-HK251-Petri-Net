#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"
#include <cmath>
#include <chrono>

using namespace std::chrono;
int main() {
    string filename = "test/example_2.pnml";

    PNMLParser parser;
    if (!parser.loadFile(filename)) {
        cout << "Failed to load file: " << filename << endl;
        return 1;
    }

    const vector<Net>& nets = parser.getNets();

    if (nets.empty()) {
        cout << "No nets found in file!" << endl;
        return 1;
    }

    cout << "Loaded " << nets.size() << " net(s) from file.\n\n";

    // ---------------------------------
    // Mỗi net là một test case riêng
    // ---------------------------------
    for (size_t i = 0; i < nets.size(); ++i) {
        cout << "======================================\n";
        cout << "NET #" << i + 1 << "   (ID = " << nets[i].id << ")\n";
        cout << "Places: " << nets[i].places.size()
             << ", Transitions: " << nets[i].transitions.size()
             << ", Arcs: " << nets[i].arcs.size() << "\n";
        cout << "======================================\n";

        // chay luon task 2 de so sanh thoi gian
        cout<<"\n--- [Task 2] Explicit computation (BFS) ---\n";

        //bat dau tinh thoi gian
        auto startBFS = high_resolution_clock::now();
        // Tạo graph và chạy BFS reachability
        Graph g(nets[i]);
        cout << "Computing reachable markings...\n";
        g.computeBFS();
        //ket thuc tinh thoi gian
        auto endBFS = high_resolution_clock::now();
        duration<double> timeBFS = endBFS - startBFS;
        g.printMarkings();
        cout << ">> BFS Execution Time: " << timeBFS.count() << " seconds.\n";



        cout << "\n--- [Task 3] Symbolic computation (BDD) ---\n";
        //bat dau tinh thoi gian
        auto startBDD = high_resolution_clock::now();
        BDDReacher bddReacher(nets[i]);
        cout << "Computing reachable markings...\n";
        bddReacher.computeBDD();
        //ket thuc tinh thoi gian
        auto endBDD = high_resolution_clock::now(); 
        duration<double> timeBDD = endBDD - startBDD;

        //in ket qua kiem thu task 3
        // BDD result = bddReacher.getReachableSet();
        bddReacher.printBDDMarkings();
        cout << ">> BDD Execution Time: " << timeBDD.count() << " seconds.\n";

        cout<<endl;
    }
    return 0;
}

//g++ -std=c++17 -Iinc src/*.cpp -o pnml_test
//pnml_test