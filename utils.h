#ifndef LAB1V2_UTILS_H
#define LAB1V2_UTILS_H

#include <string>
#include "tinyxml2.h"
#include "structs.h"
using namespace tinyxml2;

class Loader
{
public:
    Node start;
    Node goal;
    Map grid;
    std::string metrictype;
    std::string algorithm;
    int connections;
    double hweight;
    bool load_instance(const char* fileName)
    {
        XMLDocument doc;
        if(doc.LoadFile(fileName) != XMLError::XML_SUCCESS)
        {
            std::cout << "Error openning input XML file."<<std::endl;
            return false;
        }
        XMLElement* root;
        root = doc.FirstChildElement("root");
        start = Node(root->FirstChildElement("start")->IntAttribute("i"), root->FirstChildElement("start")->IntAttribute("j"));
        goal = Node(root->FirstChildElement("goal")->IntAttribute("i"), root->FirstChildElement("goal")->IntAttribute("j"));
        XMLElement* map = root->FirstChildElement("map");
        grid = Map(map->IntAttribute("width"), map->IntAttribute("height"));
        int curi(0), curj(0);
        for(auto row = map->FirstChildElement("row"); row; row = row->NextSiblingElement("row"))
        {
            std::string values = row->GetText();
            curj = 0;
            for(char value : values)
            {
                if(value == ' ')
                    continue;
                if(value == '1')
                    grid.add_obstacle(curi, curj);
                curj++;
            }
            curi++;
        }
        //grid.print();
        XMLElement* options = root->FirstChildElement("options");
        metrictype = options->Attribute("metrictype");
        algorithm = options->Attribute("algorithm");
        connections = options->IntAttribute("connections");
        hweight = options->DoubleAttribute("hweight");
        return true;
    }
};

#endif //LAB1V2_UTILS_H
