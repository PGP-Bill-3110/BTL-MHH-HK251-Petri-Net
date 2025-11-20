#include "../inc/Task2_ReachabilityGraph.h"

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
    }

    return 0;
}

//g++ -std=c++17 -Iinc src/*.cpp -o pnml_test
//pnml_test