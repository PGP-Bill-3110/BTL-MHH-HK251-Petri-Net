#include "../inc/Task2_ReachabilityGraph.h"

Graph::Graph(const Net& net) {
    places = net.places;
    transitions = net.transitions;
    arcs = net.arcs;

    initial.m.resize(places.size());
    for (int i = 0; i < places.size(); i++)
        initial.m[i] = places[i].tokens;

    buildIOMaps();
}

void Graph::buildIOMaps() {
    inputOf.clear();
    outputOf.clear();

    for (const auto& t : transitions) {
        inputOf[t.id] = {};
        outputOf[t.id] = {};
    }

    unordered_map<string,int> placeIndex;
    unordered_map<string,int> transIndex;

    for(int i=0; i<places.size(); ++i) placeIndex[places[i].id] = i;
    for(int i=0; i<transitions.size(); ++i) transIndex[transitions[i].id] = i;

    for (const auto& arc : arcs) {
        bool srcIsPlace = placeIndex.count(arc.source);
        bool trgIsPlace = placeIndex.count(arc.target);

        bool srcIsTrans = transIndex.count(arc.source);
        bool trgIsTrans = transIndex.count(arc.target);

        if (srcIsPlace && trgIsTrans) {
            // place → transition  (input arc)
            inputOf[arc.target].push_back(placeIndex[arc.source]);
        }
        else if (srcIsTrans && trgIsPlace) {
            // transition → place (output arc)
            outputOf[arc.source].push_back(placeIndex[arc.target]);
        }
        else {
            cout << "[ERROR] Invalid arc relation: "
                 << arc.source << " → " << arc.target << "\n";
        }
    }
}

// Kiểm tra missing arc dựa trên số lượng input/output
bool Graph::checkMissingArc() {
    int totalInput = 0, totalOutput = 0;

    for(const auto& t : transitions) {
        totalInput += inputOf[t.id].size();
        totalOutput += outputOf[t.id].size();
    }

    if(totalInput != totalOutput) {
        cout << "[ERROR] Total input arcs != total output arcs\n";
        return false;
    }

    return true;
}

bool Graph::isEnabled(const string& tid, const Marking& mk) {
    for (int p : inputOf[tid]) {
        if (mk.m[p] < 1) return false;
    }

    return true;
}

Marking Graph::fire(const string& tid, const Marking& mk) {
    Marking newM = mk;

    for (int p : inputOf[tid]) newM.m[p]--;
    for (int p : outputOf[tid]) newM.m[p]++;

    return newM;
}

void Graph::computeBFS() {
    if(!checkMissingArc()) {
        cout << "[FATAL] Total input/output mismatch. BFS aborted.\n";
        return;
    }

    queue<Marking> q;

    // visited.insert(initial);
    // allMarkings.push_back(initial);
    // comment phần trên để initial không trờ thành reachable marking ngay từ đầu
    q.push(initial);

    while (!q.empty()) {
        Marking cur = q.front();
        q.pop();

        bool hasEnabled = false;

        for (const auto& t : transitions) {
            if (isEnabled(t.id, cur)) {
                hasEnabled = true;
                Marking next = fire(t.id, cur);

                // cout << "Firing " << t.id << " from [";
                // for(int v: cur.m) cout << v << " ";
                // cout << "] -> [";
                // for(int v: next.m) cout << v << " ";
                // cout << "]\n";
                // cái này để in ra những transition được fire tất cả kết quả

                if (!visited.count(next)) {
                    visited.insert(next);
                    allMarkings.push_back(next);
                    q.push(next);
                }
            }
        }

        if (!hasEnabled) {
            cout << "Marking [ ";
            for (int x : cur.m) cout << x << " ";
            cout << "] has no transition enabled.\n";
        }
    }
}

void Graph::printMarkings() {
    cout << "Reachable markings: " << allMarkings.size() << "\n";
    for (int i = 0; i < (int)allMarkings.size(); i++) {
        cout << "M" << i << ": [ ";
        for (int x : allMarkings[i].m)
            cout << x << " ";
        cout << "]\n";
    }
}

const vector<Marking>& Graph::getAllMarkings() const {
    return allMarkings;
}