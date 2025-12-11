#include "../inc/Task2_ReachabilityGraph.h"

Graph::Graph(const Net &net)
{
    places = net.places;
    transitions = net.transitions;
    arcs = net.arcs;

    initial.m.resize(places.size());
    for (int i = 0; i < places.size(); i++)
        initial.m[i] = places[i].tokens;

    buildIOMaps();
}

void Graph::buildIOMaps()
{
    inputOf.clear();
    outputOf.clear();

    for (const auto &t : transitions)
    {
        inputOf[t.id] = {};
        outputOf[t.id] = {};
    }

    for (const auto &arc : arcs)
    {
        bool srcIsPlace = false;
        int placeIndex = -1;

        for (int i = 0; i < places.size(); i++)
        {
            if (places[i].id == arc.source)
            {
                srcIsPlace = true;
                placeIndex = i;
                break;
            }
        }

        if (srcIsPlace)
        {
            // source = place, target = transition
            inputOf[arc.target].push_back(placeIndex);
        }
        else
        {
            // source = transition, target = place
            for (int i = 0; i < places.size(); i++)
            {
                if (places[i].id == arc.target)
                {
                    outputOf[arc.source].push_back(i);
                    break;
                }
            }
        }
    }
}

bool Graph::checkMissingArc(const string &tid)
{
    vector<int> in = inputOf[tid];
    vector<int> out = outputOf[tid];

    return true;
}

bool Graph::isEnabled(const string &tid, const Marking &mk)
{
    // Kiểm missing arc trước
    if (!checkMissingArc(tid))
        return false;

    for (int p : inputOf[tid])
    {
        if (mk.m[p] != 1)
            return false;
    }

    return true;
}

Marking Graph::fire(const string &tid, const Marking &mk)
{
    Marking newM = mk;

    for (int p : inputOf[tid])
        newM.m[p]--;

    for (int p : outputOf[tid])
        newM.m[p]++;

    for (auto &tokens : newM.m) {
        if (tokens > 1) tokens = 1;
    }

    return newM;
}


void Graph::computeBFS()
{
    queue<Marking> q;

    visited.insert(initial);
    allMarkings.push_back(initial);
    q.push(initial);

    while (!q.empty())
    {
        Marking cur = q.front();
        q.pop();

        bool hasEnabled = false;

        for (const auto &t : transitions)
        {
            if (isEnabled(t.id, cur))
            {
                hasEnabled = true;
                Marking next = fire(t.id, cur);

                if (!visited.count(next))
                {
                    visited.insert(next);
                    allMarkings.push_back(next);
                    q.push(next);
                }
            }
        }

        if (!hasEnabled)
        {
            cout << "Marking [ ";
            for (int x : cur.m)
                cout << x << " ";
            cout << "] has no transition enabled.\n";
        }
    }
}

void Graph::printMarkings()
{
    cout << "Reachable markings: " << allMarkings.size() << "\n";
    for (int i = 0; i < (int)allMarkings.size(); i++)
    {
        cout << "M" << i << ": [ ";
        for (int x : allMarkings[i].m)
            cout << x << " ";
        cout << "]\n";
    }
}