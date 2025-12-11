#include "../inc/Task1_PNMLParser.h"

const vector<Net> &PNMLParser::getNets() const
{
    return nets;
}

bool PNMLParser::loadFile(const string &filename)
{
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(filename.c_str());
    if (eResult != XML_SUCCESS)
    {
        cerr << "Error: Cannot load PNML file: " << filename << endl;
        return false;
    }

    XMLElement *pnmlElem = doc.FirstChildElement("pnml");
    if (!pnmlElem)
    {
        cerr << "Error: No <pnml> element found." << endl;
        return false;
    }

    for (XMLElement *netElem = pnmlElem->FirstChildElement("net"); netElem; netElem = netElem->NextSiblingElement("net"))
    {
        Net net;
        net.id = netElem->Attribute("id") ? netElem->Attribute("id") : "";

        XMLElement *pageElem = netElem->FirstChildElement("page");
        if (!pageElem)
        {
            cerr << "Warning: net " << net.id << " has no <page>." << endl;
            continue;
        }

        for (XMLElement *placeElem = pageElem->FirstChildElement("place"); placeElem; placeElem = placeElem->NextSiblingElement("place"))
        {
            parsePlace(placeElem, net);
        }

        for (XMLElement *transElem = pageElem->FirstChildElement("transition"); transElem; transElem = transElem->NextSiblingElement("transition"))
        {
            parseTransition(transElem, net);
        }

        for (XMLElement *arcElem = pageElem->FirstChildElement("arc"); arcElem; arcElem = arcElem->NextSiblingElement("arc"))
        {
            parseArc(arcElem, net);
        }

        nets.push_back(net);
    }

    return true;
}

void PNMLParser::parsePlace(XMLElement *placeElem, Net &net)
{
    Place p;
    const char* idAttr = placeElem->Attribute("id");
    p.id = idAttr ? idAttr : "";

    // Parse Name an toàn
    XMLElement *nameElem = placeElem->FirstChildElement("name");
    if (nameElem) {
        XMLElement *textElem = nameElem->FirstChildElement("text");
        if (textElem && textElem->GetText())
            p.name = textElem->GetText();
    }

    p.tokens = 0;
    XMLElement *markElem = placeElem->FirstChildElement("initialMarking");
    if (markElem) {
        XMLElement *textElem = markElem->FirstChildElement("text");
        if (textElem && textElem->GetText())
            p.tokens = stoi(textElem->GetText());
    }

    XMLElement *graphicsElem = placeElem->FirstChildElement("graphics");
    if (graphicsElem) {
        XMLElement *posElem = graphicsElem->FirstChildElement("position");
        if (posElem)
        {
            posElem->QueryFloatAttribute("x", &p.pos.x);
            posElem->QueryFloatAttribute("y", &p.pos.y);
        }
    }

    net.places.push_back(p);
}

void PNMLParser::parseTransition(XMLElement *transElem, Net &net)
{
    Transition t;
    const char* idAttr = transElem->Attribute("id");
    t.id = idAttr ? idAttr : "";

    XMLElement *nameElem = transElem->FirstChildElement("name");
    if (nameElem) {
        XMLElement *textElem = nameElem->FirstChildElement("text");
        if (textElem && textElem->GetText())
            t.name = textElem->GetText();
    }

    XMLElement *graphicsElem = transElem->FirstChildElement("graphics");
    if (graphicsElem) {
        XMLElement *posElem = graphicsElem->FirstChildElement("position");
        if (posElem)
        {
            posElem->QueryFloatAttribute("x", &t.pos.x);
            posElem->QueryFloatAttribute("y", &t.pos.y);
        }
    }

    net.transitions.push_back(t);
}

void PNMLParser::parseArc(XMLElement *arcElem, Net &net)
{
    Arc a;
    const char* idAttr = arcElem->Attribute("id");
    const char* srcAttr = arcElem->Attribute("source");
    const char* tgtAttr = arcElem->Attribute("target");

    a.id = idAttr ? idAttr : "";
    a.source = srcAttr ? srcAttr : "";
    a.target = tgtAttr ? tgtAttr : "";

    a.weight = 1;
    XMLElement *insElem = arcElem->FirstChildElement("inscription");
    if (insElem) {
        XMLElement *textElem = insElem->FirstChildElement("text");
        if (textElem && textElem->GetText())
            a.weight = stoi(textElem->GetText());
    }

    net.arcs.push_back(a);
}