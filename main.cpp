/*
 *  Student:     Michael Kim
 *  Class:       CSS 449 
 *  Assignment:  Project 3
 *  Date:        5/29/2026
 */

#include <iostream>
#include <climits>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Representing a directed edge in the residual graph.
struct Edge {
    // destination node
    int to;
    int capacity;
    // completes lookup index in graph[to][revIndex]
    int revIndex;
};

// BipartiteMatcher:
//  Encapsulation of the Ford Faulkerson Algorithm adapted for 
//      maximum cardinality bipatite matching.
//  
//  -BipartiteMatcher( const &string filename ): Constructs graph from file input.
//
//  -solve(): Matches nodes using max flow and prints found matches.
class BipartiteMatcher {
private:
    // total number of nodes
    int n;
    // (n / 2)
    int half;
    int source;
    int sink;

    // node names indexed by their node # specified in the input file
    vector<string> names;

    // The bipartite graph representation:
    //  2d array, [u][v], [u] represents a given node's # as specified in the input file
    //      [v] represents outbound edges from this node.
    vector<vector<Edge>> graph; 

    // Used by bfs() to track how many steps from the source a selected node is
    vector<int> level;

    // Allows dfs() to dynamically remember where to resume traversal, preventing redundant work.
    vector<int> iter;

    // addEdge: creates and pushes struct Edge and its reverse into 
    //            vector<vector<Edge>> graph, based on input.
    void addEdge(int u, int v, int cap) {
        graph[u].push_back({v, cap, (int)graph[v].size()});
        graph[v].push_back({u, 0,   (int)graph[u].size() - 1});
    }

    // bfs: assigns BFS levels from source through residual graph.
    //      returns true if sink is reachable (augmenting path exists).
    bool bfs() {
        level.assign(graph.size(), -1);
        level[source] = 0;

        // typical queue based bfs
        queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : graph[u]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        // used as the mechanism to run our max flow algorithm outer loop
        return level[sink] >= 0;
    }

    // dfs: pushes flow from u toward sink along the level graph.
    //  iter[u] ensures each node is only visited once per phase, 
    //  ensuring O(n^2) run.
    int dfs(int u, int pushed) {
        if (u == sink) return pushed;

        for (int& i = iter[u]; i < (int)graph[u].size(); i++) {
            Edge& e = graph[u][i];

            // iterate v as parameter argument for recursive call
            int v = e.to;

            if (e.capacity > 0 && level[v] == level[u] + 1) {
                // the recursive traversal / advance() call
                int flow = dfs(v, min(pushed, e.capacity));
                if (flow > 0) {
                    // "deletion" is represented in the edge's flow being reduced to 0
                    e.capacity -= flow;
                    graph[v][e.revIndex].capacity += flow;
                    return flow;
                }
            }
        }

        return 0;
    }

    // runMaxFlow: uses bfs() to build level graph, then runs dfs() to simulate flow 
    //              being pushed down all augmenting paths. Repeats until no augmenting
    //              paths remain.
    int runMaxFlow() {
        int totalFlow = 0;

        // true as long as bfs() determines it can find a way to the sink ( level[sink] > 0 )
        while (bfs()) {
            iter.assign(graph.size(), 0);
            int flow;

            // true as long as dfs() finds augmenting path
            while ((flow = dfs(source, INT_MAX)) > 0) {
                totalFlow += flow;
            }
        }
        // total flow == total bipartite matches
        return totalFlow;
    }

public:
    
    // BipartiteMatcher: Parameterized constructor requires a string input filename
    //          and uses information from specified input file to assemble graph.
    BipartiteMatcher(const string& filename) {
        ifstream fin(filename);
        if (!fin) {
            cerr << "Error: could not open " << filename << endl;
            exit(1);
        }

        fin >> n;
        half   = n / 2;
        source = 0;
        sink   = n + 1;

        names.resize(n + 1);
        // index each name from file to a number, starting with 1
        for (int i = 1; i <= n; i++) {
            fin >> names[i];
        }

        // graph should contain: n nodes + 2 (source & sink)
        graph.assign(n + 2, vector<Edge>());

        // add edge from source to each left-side node
        for (int i = 1; i <= half; i++) {
            addEdge(source, i, 1);
        }

        // add edges between all left and right nodes
        int numEdges;
        fin >> numEdges;
        for (int i = 0; i < numEdges; i++) {
            int u, v;
            fin >> u >> v;
            addEdge(u, v, 1);
        }

        // add edge from each right-side node to sink
        for (int i = half + 1; i <= n; i++) {
            addEdge(i, sink, 1);
        }

        fin.close();
    }

    // solve: Performs the Ford Faulkerson algorithm on the assembled graph
    //           and prints the matching pairs to console.
    void solve() {
        int matches = runMaxFlow();
        
        for (int u = 1; u <= half; u++) {
            // for each left node we find the matching right node
            for (auto& e : graph[u]) {
                if (e.to >= half + 1 && e.to <= n && e.capacity == 0) {
                    cout << names[u] << " / " << names[e.to] << "\n";
                    break;
                }
            }
        }

        cout << matches << " total matches\n";
    }
};

// main(): Our program's entry point.
int main() {
    // filename can be specified in the below parameter argument
    BipartiteMatcher matcher("program3data.txt");
    matcher.solve();
    // exit program
    return 0;
}
