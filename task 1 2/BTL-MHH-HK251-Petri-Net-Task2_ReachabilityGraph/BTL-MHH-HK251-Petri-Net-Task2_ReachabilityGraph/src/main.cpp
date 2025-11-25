#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"
#include "cudd/cuddObj.hh"



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

        // Tạo graph và chạy BFS reachability
        Graph g(nets[i]);

        cout << "\nComputing reachable markings...\n";
        g.computeBFS();

        g.printMarkings();
        cout << endl;

// -------------------------------
// Task 3: Symbolic reachability using BDD
// -------------------------------
cout << "Computing symbolic reachability (BDD)...\n";

Cudd mgr;  // BDD manager
int numPlaces = nets[i].places.size();

BDD_SymbolicReach bddReach(mgr, numPlaces);

// Encode initial markings
for (const auto& marking : g.getMarkings()) {
    bddReach.encodeMarking(marking.m); // <--- dùng marking.m
}

// Compute reachable set symbolically
bddReach.computeReachableSet();

// Output BDD results
cout << "Symbolic Reachable Markings:\n";
bddReach.printReachableMarkings();
cout << "Total reachable markings (BDD): " << bddReach.getNumReachable() << "\n\n";

    }

    return 0;
}

//g++ -std=c++17 -Iinc src/*.cpp -o pnml_test
//pnml_test

// task 3: chạy trên WSL, t build thư viện CUDD rồi 
//  g++ -std=c++17 \-Iinc \-I./cudd/include \src/*.cpp \./cudd/build/libcudd.a \-lm -ldl -o pnml_test
//  ./pnml_test
