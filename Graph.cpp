#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include "Graph.h"

Graph Graph::fromStream(std::istream& pInput) {
    Graph g;
    vert_t vA;
    vert_t vB;

    pInput >> g.mVerticesCount;
    while (pInput >> vA >> vB)
        g.mEdges.emplace_back(Edge(vA, vB));

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
        std::mutex m;

        for (std::size_t constVertex = 0; constVertex < mVerticesCount; ++constVertex) {
            threads.emplace_back(
                    std::thread([&flag, &result, &m, constVertex, cnt, otherEdges](std::list<Edge> selfEdges) {
                        // Create array of vertices to permute
                        std::vector<vert_t> vertices;
                        vertices.emplace_back(constVertex);
                        for (std::size_t vert = 0; vert < cnt; ++vert)
                            if (vert != constVertex)
                                vertices.emplace_back(vert);
                        // Check every permutation until one is found
                        do {
                            std::list<Edge> edges(selfEdges);
                            for (auto& edge : edges)
                                edge.permute(vertices);

                            if (std::is_permutation(edges.begin(), edges.end(), otherEdges.begin())) {
                                std::lock_guard<std::mutex> lock(m);
                                if (flag == 0) {
                                    flag = 1;
                                    for (std::size_t v = 0; v < cnt; ++v) {
                                        result.addImage((vert_t) v, vertices[v]);
                                    }
                                }
                            }
                            if (flag != 0)
                                return;
                        } while (std::next_permutation(vertices.begin() + 1, vertices.end()));
                    }, mEdges));

        }
        for (auto&& thread : threads)
            thread.join();
    }
    return result;
}

void IsomorphicFunction::addImage(vert_t pA, vert_t pB) {
    mImage.emplace_back(std::make_pair(pA, pB));
    mExists = 1;
}

std::ostream& operator<<(std::ostream& pOut, IsomorphicFunction& pIsomorphicFunction) {
    for (auto&& e : pIsomorphicFunction.mImage) {
        pOut << std::to_string(e.first) << " --> " << std::to_string(e.second) << std::endl;
    }
    return pOut;
}
