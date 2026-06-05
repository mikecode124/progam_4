#pragma once
#include <string>
#include <vector>

// Representing a directed edge in the graph.
struct Edge {
    // destination node
    int to;
    int capacity;
    // completes lookup index in graph[to][revIndex]
    int revIndex;
};

class BipartiteMatcher {
private:
    // total number of nodes
    int n;
    // (n / 2)
    int half;
    int source;
    int sink;

    std::vector<std::string> names;
    std::vector<std::vector<Edge>> graph;
    std::vector<int> level;
    std::vector<int> iter;

    void addEdge(int u, int v, int cap);
    bool bfs();
    int dfs(int u, int pushed);
    int runMaxFlow();

public:
    BipartiteMatcher(const std::string& filename);
    void solve();
};