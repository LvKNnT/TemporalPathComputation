#include <bits/stdc++.h>

using namespace std;

// Function to sort the graph edges based on the third element (ta) in ascending order
void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

// Function to compute the earliest-arrival distance for each vertex
vector<int> ComputingForemostTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MAX); // Initialize all distances to infinity
    t[x] = ta; // Set the starting vertex's time to the given start time

    for (const auto& [u, v, t_edge, w_edge] : g) {
        // Check if the edge can be used within the given time window
        if(t_edge + w_edge <= tw && t_edge >= t[u]) {
            // Update the earliest time to reach vertex v
            if(t_edge + w_edge < t[v]) {
                t[v] = t_edge + w_edge;
            }
        }
        // Stop processing if the edge's start time exceeds the time window
        else if(t_edge >= tw) {
            break;
        }
    }

    return t;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // Get number of vertices and edges
    int n, m;
    cin >> n >> m;

    // The graph with 4-tuple: u (starting vertex), v (ending vertex), ta (begin time), tw (length of path)
    vector<tuple<int, int, int, int> > g;

    // Get starting vertex, begin time, and end time
    int x, ta, tw;
    cin >> x >> ta >> tw;
    --x; // Convert to 0-based indexing

    // Read all edges and store them in the graph
    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v; // Convert to 0-based indexing
        g.push_back({u, v, t, d});
    }

    // Sort the graph edges based on their start times
    GenerateGraph(g);

    // Compute the earliest-arrival times for all vertices
    vector<int> f = ComputingForemostTime(n, g, x, ta, tw);

    // Output the results, replacing INT_MAX with -1 for unreachable vertices
    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}