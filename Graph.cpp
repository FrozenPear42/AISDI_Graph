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
    // same vertices count?
    if (mVerticesCount != pOther.mVerticesCount)
        return result;
    // Same a=edges count?
    if (mEdges.size() != pOther.mEdges.size())
        return result;
    //Same vertices degrees?
    std::vector<vert_t> selfDegree(mVerticesCount, 0);
    std::vector<vert_t> otherDegree(mVerticesCount, 0);
    for (auto&& edge : mEdges) {
        selfDegree[edge.getA()]++;
        selfDegree[edge.getB()]++;
    }

    for (auto&& edge : pOther.mEdges) {
        otherDegree[edge.getA()]++;
        otherDegree[edge.getB()]++;
    }
    if (!std::is_permutation(selfDegree.begin(), selfDegree.end(), otherDegree.begin()))
        return result;

    std::list<Edge>& otherEdges(pOther.mEdges);
    std::list<Edge>& selfEdges(mEdges);
    std::size_t threadTurns = mVerticesCount / MAX_THREADS;
    std::size_t verticesCount = mVerticesCount;
    bool found = false;
    std::mutex m;

    for (std::size_t threadNumber = 0; threadNumber <= threadTurns; threadNumber++) {
        std::vector<std::thread> threads;
        std::size_t firstVertex = threadTurns * threadNumber;
        for (std::size_t constVertex = firstVertex; constVertex < firstVertex + mVerticesCount; ++constVertex) {
            threads.emplace_back(
                    std::thread([&found, &result, &m, &selfEdges, &otherEdges, constVertex, verticesCount]() {
                        // Create array of vertices to permute
                        std::vector<vert_t> vertices;
                        // keep first vertex fixed
                        vertices.emplace_back(constVertex);
                        for (std::size_t vertexID = 0; vertexID < verticesCount; ++vertexID)
                            if (vertexID != constVertex)
                                vertices.emplace_back(vertexID);

                        // Check each permutation until one is found
                        do {
                            std::list<Edge> edges(selfEdges);
                            for (auto& edge : edges)
                                edge.permute(vertices);
                            // check permutation and if exists acquire mutex, create function and set fond flag
                            if (std::is_permutation(edges.begin(), edges.end(), otherEdges.begin())) {
                                std::lock_guard<std::mutex> lock(m);
                                if (!found) {
                                    found = true;
                                    for (std::size_t v = 0; v < verticesCount; ++v)
                                        result.addImage((vert_t) v, vertices[v]);
                                }
                            }
                            if (found)
                                return;
                        } while (std::next_permutation(vertices.begin() + 1, vertices.end()));
                        return;
                    }));
        }
        for (auto&& thread : threads)
            thread.join();
        if (found)
            break;
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
