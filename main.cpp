#include <iostream>
#include <fstream>
#include "Graph.h"

int main(int argc, char** argv) {

    if(argc != 3){
        std::cout << "Invalid arguments\n";
        return -1;
    }

    std::ifstream g1File(argv[1]);
    std::ifstream g2File(argv[2]);

    if(!g1File || !g2File){
        std::cout << "Invalid files\n";
        return -1;
    }

    Graph g1 = Graph::fromStream(g1File);
    Graph g2 = Graph::fromStream(g2File);

    IsomorphicFunction func = g1.isomorphicTo(g2);

    if(func) {
        std::cout << "Izomorficzne\n";
        std::cout << func;
    } else {
        std::cout << "Nieizomorficzne\n";
    }

    return 0;
}