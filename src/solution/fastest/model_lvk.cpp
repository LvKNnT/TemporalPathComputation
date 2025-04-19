#include <bits/stdc++.h>

using namespace std;

// Function to sort the graph edges based on their start time (3rd element in tuple)
void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

// Function to compute the earliest-arrival distance
vector<int> ComputingForemostTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MAX); // Initialize all distances to infinity
    t[x] = ta; // Set the starting vertex's time to the given start time

    for (const auto& [u, v, t_edge, w_edge] : g) {
        // Check if the edge is within the valid time window and can be used
        if(t_edge + w_edge <= tw && t_edge >= t[u]) {
            if(t_edge + w_edge < t[v]) {
                t[v] = t_edge + w_edge; // Update the earliest arrival time for vertex v
            }
        }
        else if(t_edge >= tw) {
            break; // Stop processing if the edge's start time exceeds the time window
        }
    }

    return t;
}

// Function to compute the fastest distance using multiple passes
vector<int> ComputingFastestTimeMultiPasses(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> f(n, INT_MAX); // Initialize all distances to infinity
    f[x] = 0; // Set the starting vertex's distance to 0

    for(const auto& [u, v, t_edge, w_edge] : g) {
        // Check if the edge starts from the source and is within the valid time window
        if(u == x && ta <= t_edge && t_edge + w_edge <= tw) {
            vector<int> t = ComputingForemostTime(n, g, x, t_edge, tw);

            // Update the fastest distance for each vertex
            for(int i=0;i<n;++i) {
                if(t[i] != INT_MAX) f[i] = min(f[i], t[i] - t_edge);
            }
        }
        else if(t_edge >= tw) {
            break; // Stop processing if the edge's start time exceeds the time window
        }
    }

    return f;
}

// Class to maintain a sorted list with efficient operations
class SortedList {
private:
    vector<pair<int, int> > lst; // List of pairs sorted by both first and second values

public:
    // Function to find the largest pair whose second value is <= a
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

        if(ans == -1) return {-1, -1}; // Return invalid pair if no such element exists
        return lst[ans];
    }

    // Function to check if one pair dominates another
    bool IsDominatedFast(const pair<int, int>& u, const pair<int, int>& v) {
        // U dominates V if v.first <= u.first && u.second <= v.second
        return (u.first >= v.first && u.second <= v.second);
    }
    
    // Function to add a new pair to the sorted list while maintaining efficiency
    void AddFast(const pair<int, int>& a) {
        int i = lst.size() - 1;
        for(; i >= 0 && lst[i].first >= a.first; --i) {
            // If an existing element dominates 'a', skip insertion
            if(IsDominatedFast(lst[i], a)) {
                return;
            }
        }

        // Remove elements that are dominated by 'a'
        for(int j = lst.size() - 1; j >= 0 && lst[j].first >= a.first; --j) {
            if(IsDominatedFast(a, lst[j])) {
                lst.erase(lst.begin() + j);
            }
        }

        // Insert 'a' at the appropriate position
        lst.emplace(lst.begin() + i + 1, a);

        // Remove elements that are now dominated by 'a'
        for(; i >= 0; --i) {
            if(IsDominatedFast(a, lst[i])) lst.erase(lst.begin() + i);
            else break; // Stop when encountering a non-dominated element
        }
    }
};

// Function to compute the fastest distance
vector<int> ComputingFastestTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> f(n, INT_MAX); // Initialize all distances to infinity
    f[x] = 0; // Set the starting vertex's distance to 0

    vector<SortedList> L(n); // Create a sorted list for each vertex

    for(const auto& [u, v, t_edge, w_edge] : g) {
        // Check if the edge is within the valid time window
        if(t_edge >= ta && t_edge + w_edge <= tw) {
            if(u == x) {
                L[x].AddFast({t_edge, t_edge}); // Add the edge to the sorted list of the source vertex
            }

            pair<int, int> cur = L[u].UpperBound(t_edge); // Find the best pair for the current edge
            if(cur.first == -1) continue; // Skip if no valid pair exists
            
            cur.second = t_edge + w_edge; // Update the second value of the pair

            L[v].AddFast(cur); // Add the updated pair to the sorted list of the destination vertex
            
            if(cur.second - cur.first < f[v]) {
                f[v] = cur.second - cur.first; // Update the fastest distance for vertex v
            }
        }
        else if(t_edge >= tw) {
            break; // Stop processing if the edge's start time exceeds the time window
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

    // The graph with 4-tuple: u (starting vertex), v (ending vertex), ta (begin time), tw (length of path)
    vector<tuple<int, int, int, int> > g;

    // Get starting vertex, begin time, and end time
    int x, ta, tw;
    cin >> x >> ta >> tw;
    --x; // Convert to 0-based index

    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v; // Convert to 0-based index
        g.push_back({u, v, t, d});
    }

    GenerateGraph(g); // Sort the graph edges by start time

    vector<int> f = ComputingFastestTime(n, g, x, ta, tw); // Compute the fastest distances

    for(int i=0; i<n; ++i) {
        // Output the result, replacing INT_MAX with -1 for unreachable vertices
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}
