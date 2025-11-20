#include <iostream>
#include "../inc/Task1_PNMLParser.h"

using namespace std;

int main() {
    PNMLParser parser;

    string filename = "test/example.pnml";
    if (!parser.loadFile(filename)) {
        cerr << "Failed to load file: " << filename << endl;
        return 1;
    }

    const vector<Net>& nets = parser.getNets();

    cout << "========== PNML PARSER TEST ==========\n";
    cout << "Total nets found: " << nets.size() << "\n\n";

    for (size_t i = 0; i < nets.size(); i++) {
        const Net& net = nets[i];
        cout << "---------------------------------------\n";
        cout << "NET #" << (i + 1) << "   ID = " << net.id << "\n";
        cout << "---------------------------------------\n";

        // PLACES
        cout << "Places: " << net.places.size() << "\n";
        for (const auto& p : net.places) {
            cout << "  - ID: " << p.id
                 << ", name: " << p.name
                 << ", tokens: " << p.tokens
                 << ", pos=(" << p.pos.x << ", " << p.pos.y << ")\n";
        }
        cout << endl;

        // TRANSITIONS
        cout << "Transitions: " << net.transitions.size() << "\n";
        for (const auto& t : net.transitions) {
            cout << "  - ID: " << t.id
                 << ", name: " << t.name
                 << ", pos=(" << t.pos.x << ", " << t.pos.y << ")\n";
        }
        cout << endl;

        // ARCS
        cout << "Arcs: " << net.arcs.size() << "\n";
        for (const auto& a : net.arcs) {
            cout << "  - ID: " << a.id
                 << ", source: " << a.source
                 << ", target: " << a.target
                 << ", weight: " << a.weight << "\n";
        }

        cout << "\n=======================================\n\n";
    }

    return 0;
}

//g++ -std=c++17 -Iinc src/*.cpp -o pnml_test
//./pnml_test