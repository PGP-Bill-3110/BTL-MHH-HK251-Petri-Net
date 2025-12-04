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

    XMLElement* pnmlElem = doc.FirstChildElement("pnml");
    if (!pnmlElem) {
        cerr << "Error: No <pnml> element found." << endl;
        return false;
    }

    for(XMLElement* netElem = pnmlElem->FirstChildElement("net"); netElem; netElem = netElem->NextSiblingElement("net")) {
        Net net;
        net.id = netElem->Attribute("id") ? netElem->Attribute("id") : "";

        XMLElement* pageElem = netElem->FirstChildElement("page");
        if(!pageElem){
            cerr << "Warning: net " << net.id << " has no <page>." << endl;
            continue;
        }

        for(XMLElement* placeElem = pageElem->FirstChildElement("place"); placeElem; placeElem = placeElem->NextSiblingElement("place")) {
            parsePlace(placeElem, net);
        }

        for(XMLElement* transElem = pageElem->FirstChildElement("transition"); transElem; transElem = transElem->NextSiblingElement("transition")) {
            parseTransition(transElem, net);
        }

        for(XMLElement* arcElem = pageElem->FirstChildElement("arc"); arcElem; arcElem = arcElem->NextSiblingElement("arc")) {
            parseArc(arcElem, net);
        }

        nets.push_back(net);
    }

    return true;
}

void PNMLParser::parsePlace(XMLElement* placeElem, Net& net) {
    Place p;
    p.id = placeElem->Attribute("id");

    XMLElement* nameElem = placeElem->FirstChildElement("name")->FirstChildElement("text");
    if (nameElem && nameElem->GetText()) p.name = nameElem->GetText();

    XMLElement* markElem = placeElem->FirstChildElement("initialMarking")->FirstChildElement("text");
    if (markElem && markElem->GetText()) p.tokens = stoi(markElem->GetText());

    XMLElement* posElem = placeElem->FirstChildElement("graphics")->FirstChildElement("position");
    if (posElem) {
        posElem->QueryFloatAttribute("x", &p.pos.x);
        posElem->QueryFloatAttribute("y", &p.pos.y);
    }

    net.places.push_back(p);
}

void PNMLParser::parseTransition(XMLElement* transElem, Net& net) {
    Transition t;
    t.id = transElem->Attribute("id");

    XMLElement* nameElem = transElem->FirstChildElement("name")->FirstChildElement("text");
    if (nameElem && nameElem->GetText()) t.name = nameElem->GetText();

    XMLElement* posElem = transElem->FirstChildElement("graphics")->FirstChildElement("position");
    if (posElem) {
        posElem->QueryFloatAttribute("x", &t.pos.x);
        posElem->QueryFloatAttribute("y", &t.pos.y);
    }

    net.transitions.push_back(t);
}

void PNMLParser::parseArc(XMLElement* arcElem, Net& net) {
    Arc a;
    a.id = arcElem->Attribute("id");
    a.source = arcElem->Attribute("source");
    a.target = arcElem->Attribute("target");

    XMLElement* insElem = arcElem->FirstChildElement("inscription")->FirstChildElement("text");
    if (insElem && insElem->GetText()) a.weight = stoi(insElem->GetText());

    net.arcs.push_back(a);
}