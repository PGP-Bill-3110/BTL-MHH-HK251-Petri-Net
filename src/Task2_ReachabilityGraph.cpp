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

    for (const auto& arc : arcs) {
        bool srcIsPlace = false;
        int placeIndex = -1;

        for (int i = 0; i < places.size(); i++) {
            if (places[i].id == arc.source) {
                srcIsPlace = true;
                placeIndex = i;
                break;
            }
        }

        if (srcIsPlace) {
            // source = place, target = transition
            inputOf[arc.target].push_back(placeIndex);
        } else {
            // source = transition, target = place
            for (int i = 0; i < places.size(); i++) {
                if (places[i].id == arc.target) {
                    outputOf[arc.source].push_back(i);
                    break;
                }
            }
        }
    }
}

// Kiểm tra missing arc dựa trên số lượng input/output
bool Graph::checkMissingArc(const string& tid) {
    // Nếu một transition không có input hoặc output arcs → missing
    if (inputOf[tid].empty() || outputOf[tid].empty()) {
        cout << "[ERROR] Transition " << tid << " has missing arc(s).\n";
        return false;
    }

    // Nếu số lượng input arcs != số lượng output arcs sau loại trùng lặp → missing
    vector<int> in = inputOf[tid];
    vector<int> out = outputOf[tid];

    if (in.size() != out.size()) {
        cout << "[ERROR] Transition " << tid << " input/output mismatch (possible missing arc).\n";
        return false;
    }

    return true;
}

bool Graph::isEnabled(const string& tid, const Marking& mk) {
    // Kiểm missing arc trước
    if (!checkMissingArc(tid)) return false;

    for (int p : inputOf[tid]) {
        if (mk.m[p] != 1) return false;
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
    queue<Marking> q;

    visited.insert(initial);
    allMarkings.push_back(initial);
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
                // cái này là cái reachable markings sẽ thấy nó bị ngược
                // nhưng mà không phải đâu tại reachable markings của T2 nó trùng initial markings
                //nên không có in lại, có thể bỏ comment để check :))))

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