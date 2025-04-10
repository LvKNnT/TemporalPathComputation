#include <bits/stdc++.h>

using namespace std;

// Function to sort the graph edges based on the third element (ta)
void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

class SortedList {
private:
    vector<pair<int, int> > lst; // Stores pairs of (shortest distance, time)

public:
    // Finds the largest element in the list whose second value is <= a
    pair<int, int> UpperBound(const int& a) {
        int l = 0, r = lst.size() - 1;
        int ans = -1;

        while(l <= r) {
            int m = (l + r) / 2;

            if(lst[m].second <= a) {
                ans = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        if(ans == -1) return {-1, -1}; // No valid element found
        return lst[ans];
    }

    // Checks if u is dominated by v (both distance and time are less than or equal)
    bool IsDominatedShort(const pair<int, int>& u, const pair<int, int>& v) {
        return (u.first <= v.first && u.second <= v.second);
    }

    // Adds a new pair to the list while maintaining the sorted and non-dominated property
    void AddShort(const pair<int, int>& a) {
        int i = lst.size() - 1;
        for(; i >= 0 && lst[i].first <= a.first; --i) {
            if(IsDominatedShort(lst[i], a)) {
                return; // If the new pair is dominated, do not add it
            }
        }

        // Remove all elements dominated by the new pair
        for(int j = lst.size() - 1; j >= 0 && lst[j].first <= a.first; --j) {
            if(IsDominatedShort(a, lst[j])) {
                lst.erase(lst.begin() + j);
            }
        }
        lst.emplace(lst.begin() + i + 1, a); // Insert the new pair in the correct position
        for(; i >= 0; --i) {
            if(IsDominatedShort(a, lst[i])) lst.erase(lst.begin() + i); // Remove dominated elements
            else break;
        }
    }
};

// Computes the shortest time to reach each vertex from the starting vertex
vector<int> ComputingShortestTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> f(n, INT_MAX); // Initialize distances to infinity
    f[x] = 0; // Distance to the starting vertex is 0
    
    vector<SortedList> L(n); // List to store non-dominated pairs for each vertex

    for(const auto& [u, v, t_edge, w_edge] : g) {
        // Check if the edge is within the valid time range
        if(t_edge >= ta && t_edge + w_edge <= tw) {
            if(u == x) {
                L[x].AddShort({0, t_edge}); // Add the starting vertex's initial state
            }

            pair<int, int> cur = L[u].UpperBound(t_edge); // Find the best state to use for this edge
            if(cur.first == -1) continue; // No valid state found

            cur.first += w_edge; // Update distance
            cur.second = t_edge + w_edge; // Update time

            L[v].AddShort(cur); // Add the new state to the destination vertex

            if(cur.first < f[v]) {
                f[v] = cur.first; // Update the shortest distance to the destination vertex
            }
        }
        else if(t_edge >= tw) {
            break; // Stop processing edges if the time exceeds the valid range
        }
    }

    return f;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // Get number of vertices and edges
    int n, m;
    cin >> n >> m;

    // The graph with 4 tuple: u (starting vertex), v (ending vertex), ta (begin time), tw (length of path)
    vector<tuple<int, int, int, int> > g;

    // Get starting vertex, begin time, and end time
    int x, ta, tw;
    cin >> x >> ta >> tw;
    --x;

    // Read the edges of the graph
    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v;
        g.push_back({u, v, t, d});
    }

    GenerateGraph(g); // Sort the graph edges by start time

    vector<int> f = ComputingShortestTime(n, g, x, ta, tw); // Compute the shortest times

    // Output the results
    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}