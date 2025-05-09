#include <bits/stdc++.h>

using namespace std;

// Function to sort the graph edges based on the third element (ta) in ascending order
void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

// Function to compute the latest-departure time (reverse foremost time)
vector<int> ComputingReverseForemostTime(const int& n, vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MIN); // Initialize all times to INT_MIN
    t[x] = tw; // Set the starting vertex's time to the given tw

    // Traverse the graph edges in reverse order
    for(int i = g.size() - 1; i >= 0; --i) {
        const auto& [u, v, t_edge, w_edge] = g[i]; // Extract edge details

        // Check if the edge's start time is valid
        if(t_edge >= ta) {
            // Update the time for vertex u if conditions are met
            if(t_edge + w_edge <= t[v]) {
                if(t_edge > t[u]) {
                    t[u] = t_edge;
                }
            }
        }
        else break; // Stop if the edge's start time is less than ta
    }

    return t; // Return the computed times
}

signed main() {
    ios_base::sync_with_stdio(false); // Optimize input/output
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
    --x; // Convert to 0-based index

    // Read all edges of the graph
    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v; // Convert to 0-based index
        g.push_back({u, v, t, d});
    }

    // Sort the graph edges based on the begin time (ta)
    GenerateGraph(g);

    // Compute the reverse foremost time for all vertices
    vector<int> f = ComputingReverseForemostTime(n, g, x, ta, tw);

    // Output the results
    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MIN ? -1 : f[i]) << " \n"[i+1 == n];
    }
}