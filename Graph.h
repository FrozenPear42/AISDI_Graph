#ifndef AISDI_GRAPH_GRAPH_H
#define AISDI_GRAPH_GRAPH_H

#include <istream>
#include <vector>
#include <list>

typedef uint32_t vert_t;

class IsomorphicFunction {
public:
    IsomorphicFunction() : mExists(false) {};

    bool exists() { return mExists; };

    operator bool() { return mExists; }

    void addImage(vert_t pA, vert_t pB);

    friend std::ostream& operator<<(std::ostream& pOut, IsomorphicFunction& pIsomorphicFunction);

private:
    bool mExists;
    std::vector<std::pair<vert_t, vert_t>> mImage;
};

class Edge {
public:
    Edge(vert_t pA, vert_t pB) : mPair(std::make_pair(pA, pB)) {}

    bool operator==(const Edge& other) const {
        return (mPair.first == other.mPair.first && mPair.second == other.mPair.second) ||
               (mPair.first == other.mPair.second && mPair.second == other.mPair.first);
    }

    bool operator!=(const Edge& other) const {
        return !operator==(other);
    }

    void permute(std::vector<vert_t>& pVertices) {
        mPair.first = pVertices[mPair.first];
        mPair.second = pVertices[mPair.second];
    }

private:
    std::pair<vert_t, vert_t> mPair;
};

class Graph {
public:
    Graph() : mVerticesCount(0) {};

    static Graph fromStream(std::istream& pInput);

    IsomorphicFunction isomorphicTo(Graph& pOther);

private:
    std::size_t mVerticesCount;
    std::list<Edge> mEdges;
};

#endif //AISDI_GRAPH_GRAPH_H
