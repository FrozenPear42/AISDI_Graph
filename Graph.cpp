#include <thread>
#include <algorithm>
#include <iostream>
#include "Graph.h"

Graph Graph::fromStream(std::istream& pInput) {
    Graph g;

    pInput >> g.mVerticesCount;

    while (pInput.good()) {
        vert_t vA;
        vert_t vB;
        pInput >> vA >> vB;
        g.mEdges.emplace_back(Edge(vA, vB));
    }

    return g;
}

IsomorphicFunction Graph::isomorphicTo(Graph& pOther) {
    IsomorphicFunction result;
    if (mVerticesCount != pOther.mVerticesCount)
        return result;
    if (mEdges.size() != pOther.mEdges.size())
        return result;

    std::vector<std::thread> threads;
    uint8_t flag = 0;
    //thread for each vertex
    if (mVerticesCount <= 20) {
        std::size_t cnt = mVerticesCount;
        std::list<Edge> otherEdges(pOther.mEdges);
        for (std::size_t i = 0; i < mVerticesCount; ++i) {
            threads.emplace_back(std::thread([&flag, i, cnt, otherEdges](std::list<Edge> edges) {
                std::vector<vert_t> vertices;
                vertices.emplace_back(i);
                for (std::size_t vert = 0; vert < cnt; ++vert)
                    if (vert != i)
                        vertices.emplace_back(vert);
                do {
                    for (auto& edge : edges) {
                        edge.permute(vertices);
                    }
                    if (std::is_permutation(edges.begin(), edges.end(), otherEdges.begin()))
                        flag = 1;

                    if (flag != 0)
                        return;
                } while (std::next_permutation(vertices.begin() + 1, vertices.end()));
            }, mEdges));

        }
        for (auto&& e : threads)
            e.join();
    }
    if(flag != 0)
        result.addImage(1, 1);
    return result;
}

void IsomorphicFunction::addImage(vert_t pA, vert_t pB) {
    mImage.emplace_back(std::make_pair(pA, pB));
    mExists = 1;
}

std::ostream& operator<<(std::ostream& pOut, IsomorphicFunction& pIsomorphicFunction) {
    for (auto&& e : pIsomorphicFunction.mImage) {
        pOut << std::to_string(e.first) << " " << std::to_string(e.second) << std::endl;
    }
    return pOut;
}
