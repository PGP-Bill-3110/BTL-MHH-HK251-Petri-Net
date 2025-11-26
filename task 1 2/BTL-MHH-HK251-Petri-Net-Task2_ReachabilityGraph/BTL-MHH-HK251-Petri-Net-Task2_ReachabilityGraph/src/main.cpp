#include "../inc/Task2_ReachabilityGraph.h"
#include "../inc/Task3_BDD_SymbolicReach.h"

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

        cout << "\n--- [Task 2] Explicit Reachability (BFS) ---\n";
        // Tạo graph và chạy BFS reachability
        Graph g(nets[i]);
        cout << "\nComputing reachable markings...\n";
        g.computeBFS();
        g.printMarkings();
        cout << endl;
    }

    //Task  3
        for (size_t i = 0; i < nets.size(); ++i) {
        cout << "======================================\n";
        cout << "NET #" << i + 1 << "   (ID = " << nets[i].id << ")\n";
        cout << "Places: " << nets[i].places.size()
             << ", Transitions: " << nets[i].transitions.size()
             << ", Arcs: " << nets[i].arcs.size() << "\n";
        cout << "======================================\n";

        cout << "\n--- [Task 3] Symbolic Reachability (BDD) ---\n";
       
        cout << "\nComputing reachable markings...\n";

        // BDDReacher bddReacher(nets[i]);
        // bddReacher.computeBDD();

            try {
            // Scope block { } ép buộc hủy BDD Manager ngay sau khi dùng xong
            {
                BDDReacher bddReacher(nets[i]);
                bddReacher.computeBDD();
            }
        } 
        catch (const std::exception& e) {
            cerr << "[ERROR] Task 3 failed for Net #" << (i + 1) << ": " << e.what() << endl;
        }


        cout << endl;
    }

    return 0;
}

// task 3:

// g++ -std=c++17 \
-Iinc \
-I./cudd/include \
src/*.cpp \
./cudd/build/libcudd.a \
-lm -ldl \
-o pnml_test


//   ./pnml_test