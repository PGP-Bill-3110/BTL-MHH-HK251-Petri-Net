#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

int main() {
    string filename = "test/phylosopher.pnml";

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

        // -----------------------------
        // Task 2: BFS Explicit Reachability
        // -----------------------------
        cout << "\n--- [Task 2] Explicit computation (BFS) ---\n";
        auto startBFS = high_resolution_clock::now();

        Graph g(nets[i]);
        cout << "Computing reachable markings...\n";
        g.computeBFS();

        auto endBFS = high_resolution_clock::now();
        duration<double> timeBFS = endBFS - startBFS;

        g.printMarkings();
        cout << ">> BFS Execution Time: " << timeBFS.count() << " seconds.\n";

        // -----------------------------
        // Task 3: BDD Symbolic Reachability
        // -----------------------------
        cout << "\n--- [Task 3] Symbolic computation (BDD) ---\n";
        auto startBDD = high_resolution_clock::now();

        BDDReacher bddReacher(nets[i]);
        cout << "Computing reachable markings...\n";
        bddReacher.computeBDD();

        auto endBDD = high_resolution_clock::now();
        duration<double> timeBDD = endBDD - startBDD;

        bddReacher.printBDDMarkings();
        cout << ">> BDD Execution Time: " << timeBDD.count() << " seconds.\n";

        cout << endl;
    }

    return 0;
}

// compile:  

// g++ -std=c++17 \
-Iinc \
-Icudd/include \
src/*.cpp \
cudd/build/libcudd.a \
-lm -ldl \
-o pnml_test

// run: ./pnml_test

    
