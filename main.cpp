#include <iostream>
#include <list>
#include <set>
#include "structs.h"
#include "utils.h"
#include <chrono>

class BFS //breadth-first-search
{
public:
    Result find_path(Node start, Node goal, Map grid)
    {
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result;
        int steps = 0;
        start.g = 0;
        std::list<Node> OPEN;
        OPEN.push_back(start);
        std::set<Node> CLOSED;
        CLOSED.insert(start);
        bool pathfound = false;
        while(!OPEN.empty() && !pathfound)
        {
           Node current = OPEN.front();
           OPEN.pop_front();
           steps++;
           auto neighbors = grid.get_neighbors(current);
           for(auto n:neighbors) {
               if (CLOSED.find(n) == CLOSED.end())
               {
                   n.g = current.g + 1;
                   n.parent = &(*CLOSED.find(current));
                   OPEN.push_back(n);
                   CLOSED.insert(n);
                   if(n == goal) {
                       result.path = reconstruct_path(n);
                       result.cost = n.g;
                       pathfound = true;
                       break;
                    }
                }
            }
        }
        result.steps = steps;
        result.nodes_created = CLOSED.size();
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count()/1e+9;
        return result;
    }
    std::list<Node> reconstruct_path(Node n)
    {
        std::list<Node> path;
        while(n.parent != nullptr)
        {
            path.push_front(n);
            n = *n.parent;
        }
        path.push_front(n);
        return path;
    }
};

class AStar
{
public:
    Result find_path(Node start, Node goal, Map grid, std::string metrictype="Octile", int connections=8, double hweight=1)
    {
        //TODO - implement the main cycle of AStar algorithm
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result;
        int steps = 0;
        start.g = 0;
        std::list <Node> OPEN;
        OPEN.push_back(start);
        std::set <Node> CLOSED;
        CLOSED.insert(start);
        bool pathfound = false;
        while (!OPEN.empty() && !pathfound) {
            Node current = OPEN.front();
            for (auto n : OPEN) {
                if (n.f < current.f)
                    current = n;
            }
            OPEN.remove(current);
            steps++;
            auto neighbors = grid.get_neighbors(current);
            CLOSED.insert(current);
            for (auto n : neighbors) {
                if (CLOSED.find(n) == CLOSED.end()) {
                    if (n.i != current.i && n.j != current.j)
                        n.g = current.g + 1.41421356237;
                    else
                        n.g = current.g + 1;
                    n.h = hweight + count_h_value(n, goal, metrictype);
                    n.f = n.g + n.h;
                    n.parent = &(*CLOSED.find(current));
                    OPEN.push_back(n);
                    if (n == goal) {
                        result.path = reconstruct_path(n);
                        result.cost = n.g;
                        pathfound = true;
                        break;
                    }
                }
            }
        }
        result.steps = steps;
        result.nodes_created = CLOSED.size();
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count() / 1e+9;
        return result;
    }
    double count_h_value(Node current, Node goal, std::string metrictype="Octile")
    {
        //TODO - add support of all three metrics
        int ai = current.i;
        int aj = current.j;
        int bi = goal.i;
        int bj = goal.j;
        double dx = abs(ai - bi);
        double dy = abs(aj - bj);
        double answer;
        int heuristic_type = 5;
        if (metrictype == "Euclidean") heuristic_type = 0;
        else if (metrictype == "Manhattan") heuristic_type = 1;
        else if (metrictype == "Octile") heuristic_type = 2;
        switch (heuristic_type) {
        case 0:
            answer = sqrt(pow(ai - bi, 2) + pow(aj - bj, 2));
            break;
        case 1:
            answer = dx + dy;
            break;
        case 2:
            answer = abs(dx - dy) + (sqrt(2)) * std::min(dx, dy);
            break;
        default:
            answer = 0;
            break;
        }
        return answer;
    }
    std::list<Node> reconstruct_path(Node n)
    {
        //TODO - reconstruct path using back pointers
        std::list<Node> path;
        while (n.parent != nullptr) {
            path.push_front(n);
            n = *n.parent;
        }
        path.push_front(n);
        return path;
    }
};

int main(int argc, char* argv[]) //argc - argumnet counter, argv - argument values
{
    for(int i=0; i<argc; i++)
        std::cout<<argv[i]<<"\n";
    if(argc<2)
    {
        std::cout << "Name of the input XML file is not specified."<<std::endl;
        return 1;
    }
    Loader loader;
    loader.load_instance(argv[1]);
    Result result;
    if(loader.algorithm == "BFS")
    {
        BFS bfs;
        result = bfs.find_path(loader.start, loader.goal, loader.grid);
    }
    else
    {
        if(loader.algorithm == "Dijkstra")
            loader.hweight = 0;
        AStar astar;
        result = astar.find_path(loader.start, loader.goal, loader.grid, loader.metrictype, loader.connections, loader.hweight);
    }
    loader.grid.print(result.path);
    std::cout<<"Cost: "<<result.cost<<"\nRuntime: "<<result.runtime
    <<"\nSteps: "<<result.steps<<"\nNodes created: "<<result.nodes_created<<std::endl;
    return 0;
}