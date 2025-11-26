#ifndef TASK1_PNMLPARSER_H
#define TASK1_PNMLPARSER_H

#include "main.h"

struct Position {
    float x, y;
};

struct Place {
    string id;
    string name;
    int tokens;
    Position pos;
};

struct Transition {
    string id;
    string name;
    Position pos;
};

struct Arc {
    string id;
    string source;
    string target;
    int weight;
};

struct Net {
    string id;
    vector<Place> places;
    vector<Transition> transitions;
    vector<Arc> arcs;
};

class PNMLParser {
public:
    PNMLParser() = default;
    ~PNMLParser() = default;

    bool loadFile(const string& filename);
    const vector<Net>& getNets() const;

private:
    vector<Net> nets;

    void parsePlace(XMLElement* placeElem, Net& net);
    void parseTransition(XMLElement* transElem, Net& net);
    void parseArc(XMLElement* arcElem, Net& net);
};

#endif
