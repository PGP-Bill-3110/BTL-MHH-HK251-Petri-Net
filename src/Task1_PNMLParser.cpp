#include "../inc/Task1_PNMLParser.h"

const vector<Net>& PNMLParser::getNets() const {
    return nets;
}

bool PNMLParser::loadFile(const string& filename) {
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(filename.c_str());
    if (eResult != XML_SUCCESS) {
        cerr << "Error: Cannot load PNML file: " << filename << endl;
        return false;
    }

    XMLElement* pnmlElem = doc.RootElement();
    if (!pnmlElem) {
        cerr << "Error: No root element <pnml> found." << endl;
        return false;
    }

    for (XMLElement* netElem = pnmlElem->FirstChildElement(); netElem; netElem = netElem->NextSiblingElement()) {
        if (string(netElem->Name()).find("net") == string::npos) continue;

        Net net;
        net.id = netElem->Attribute("id") ? netElem->Attribute("id") : "";

        XMLElement* pageElem = nullptr;
        for (XMLElement* p = netElem->FirstChildElement(); p; p = p->NextSiblingElement()) {
            if (string(p->Name()).find("page") != string::npos) {
                pageElem = p;
                break;
            }
        }
        if (!pageElem) {
            cerr << "Warning: net " << net.id << " has no <page> element." << endl;
            continue;
        }

        for (XMLElement* placeElem = pageElem->FirstChildElement(); placeElem; placeElem = placeElem->NextSiblingElement()) {
            if (string(placeElem->Name()).find("place") != string::npos) parsePlace(placeElem, net);
        }

        for (XMLElement* transElem = pageElem->FirstChildElement(); transElem; transElem = transElem->NextSiblingElement()) {
            if (string(transElem->Name()).find("transition") != string::npos) parseTransition(transElem, net);
        }

        for (XMLElement* arcElem = pageElem->FirstChildElement(); arcElem; arcElem = arcElem->NextSiblingElement()) {
            if (string(arcElem->Name()).find("arc") != string::npos) parseArc(arcElem, net);
        }

        nets.push_back(net);
    }

    return true;
}

void PNMLParser::parsePlace(XMLElement* placeElem, Net& net) {
    Place p;
    p.id = placeElem->Attribute("id") ? placeElem->Attribute("id") : "";

    for (XMLElement* c = placeElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("name") != string::npos) {
            XMLElement* textElem = c->FirstChildElement("text");
            if (textElem && textElem->GetText()) p.name = textElem->GetText();
        }
    }

    for (XMLElement* c = placeElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("initialMarking") != string::npos) {
            XMLElement* textElem = c->FirstChildElement("text");
            if (textElem && textElem->GetText()) p.tokens = stoi(textElem->GetText());
        }
    }

    for (XMLElement* c = placeElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("graphics") != string::npos) {
            XMLElement* posElem = c->FirstChildElement("position");
            if (posElem) {
                posElem->QueryFloatAttribute("x", &p.pos.x);
                posElem->QueryFloatAttribute("y", &p.pos.y);
            }
        }
    }

    net.places.push_back(p);
}

void PNMLParser::parseTransition(XMLElement* transElem, Net& net) {
    Transition t;
    t.id = transElem->Attribute("id") ? transElem->Attribute("id") : "";

    for (XMLElement* c = transElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("name") != string::npos) {
            XMLElement* textElem = c->FirstChildElement("text");
            if (textElem && textElem->GetText()) t.name = textElem->GetText();
        }
    }

    for (XMLElement* c = transElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("graphics") != string::npos) {
            XMLElement* posElem = c->FirstChildElement("position");
            if (posElem) {
                posElem->QueryFloatAttribute("x", &t.pos.x);
                posElem->QueryFloatAttribute("y", &t.pos.y);
            }
        }
    }

    net.transitions.push_back(t);
}

void PNMLParser::parseArc(XMLElement* arcElem, Net& net) {
    Arc a;
    a.id = arcElem->Attribute("id") ? arcElem->Attribute("id") : "";
    a.source = arcElem->Attribute("source") ? arcElem->Attribute("source") : "";
    a.target = arcElem->Attribute("target") ? arcElem->Attribute("target") : "";
    a.weight = 1;

    for (XMLElement* c = arcElem->FirstChildElement(); c; c = c->NextSiblingElement()) {
        if (string(c->Name()).find("inscription") != string::npos) {
            XMLElement* textElem = c->FirstChildElement("text");
            if (textElem && textElem->GetText()) a.weight = stoi(textElem->GetText());
        }
    }

    net.arcs.push_back(a);
}
