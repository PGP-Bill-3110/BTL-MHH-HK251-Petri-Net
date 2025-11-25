#include "Task1_PNMLParser.h"
#include <unordered_set>
#include <queue>
#include <set>

struct Marking {
    vector<int> m;

    bool operator==(const Marking& other) const {
        return m == other.m;
    }
};

struct MarkingHash {
    size_t operator()(const Marking& mk) const {
        size_t h = 0;
        for (int x : mk.m)
            h = h * 1315423911u + x; //Jenkens hash
        return h;
    }
};

class Graph {
public:
    Graph(const Net& net);

    void computeBFS();
    void printMarkings();
    const vector<Marking>& getMarkings() const {
        return allMarkings;
    }
private:
    vector<Place> places;
    vector<Transition> transitions;
    vector<Arc> arcs;

    // mapping transition → input places
    unordered_map<string, vector<int>> inputOf;
    // mapping transition → output places
    unordered_map<string, vector<int>> outputOf;

    Marking initial;
    vector<Marking> allMarkings;
    unordered_set<Marking, MarkingHash> visited;

    void buildIOMaps();
    bool checkMissingArc(const std::string& tid);
    bool isEnabled(const string& tid, const Marking& mk);
    Marking fire(const string& tid, const Marking& mk);
};