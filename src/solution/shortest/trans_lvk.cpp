#include <bits/stdc++.h>

using namespace std;

namespace TransformingGraph {
    int n;
    int vertices_count = 0, edges_count = 0;
    vector<pair<int, int> > gi; // Stores unique (vertex, time) pairs
    vector<vector<tuple<int, int> > > g; // Adjacency list for the transformed graph
    vector<vector<int>> ins, outs; // Stores incoming and outgoing times for each vertex

    // Get the ID of a vertex-time pair in the transformed graph
    int getId(int u, int ta) {
        auto it = lower_bound(gi.begin(), gi.end(), make_pair(u, ta));
        if(it != gi.end() && it->first == u && it->second == ta) {
            return it - gi.begin();
        }
        assert(false); // Should not reach here if the pair exists
        return -1;
    }

    // Generate the transformed graph from the original graph
    void GenerateGraph(const int& n, const vector<tuple<int, int, int, int> >& old_g) {
        TransformingGraph::n = n;
        map<pair<int, int>, bool> has_in, has_out, has_i;

        ins.assign(n, vector<int>()); // Initialize incoming times
        outs.assign(n, vector<int>()); // Initialize outgoing times

        // Process edges to populate incoming and outgoing times
        for(const auto& [u, v, ta, tw] : old_g) {
            if(has_out.find({u, ta}) == has_out.end()) {
                outs[u].push_back(ta);
                has_out[{u, ta}] = true;
            }
            if(has_in.find({v, ta + tw}) == has_in.end()) {
                ins[v].push_back(ta + tw);
                has_in[{v, ta + tw}] = true;
            }

            has_i[{u, ta}] = true; // Mark vertex-time pair as existing
            has_i[{v, ta + tw}] = true;
        }

        {
            int i = 0;
            for(const auto& p : has_i) {
                if(p.second) {
                    gi.push_back(p.first); // Add unique vertex-time pairs to gi
                    ++i;
                }
            }

            vertices_count = gi.size(); // Total number of vertices in the transformed graph
            g.assign(vertices_count, vector<tuple<int, int> >()); // Initialize adjacency list
        }

        // Add edges to the transformed graph
        for(const auto& [u, v, ta, tw] : old_g) {
            ++edges_count;
            g[getId(u, ta)].push_back({getId(v, ta + tw), tw});
        }
        
        // Add edges between consecutive incoming times
        for(int i=0;i<n;++i) {
            sort(ins[i].begin(), ins[i].end());
            sort(outs[i].begin(), outs[i].end());

            for(int j=1;j<ins[i].size();++j) {
                ++edges_count;
                g[getId(i, ins[i][j - 1])].push_back({getId(i, ins[i][j]), 0});
            }
            
            // Add edges between consecutive outgoing times
            for(int j=1;j<outs[i].size();++j) {
                ++edges_count;
                g[getId(i, outs[i][j - 1])].push_back({getId(i, outs[i][j]), 0});
            }

            // Add edges between the latest incoming time and the earliest outgoing time
            int l = ins[i].size() - 1, r = outs[i].size() - 1;
            while (l >= 0 && r >= 0) {
                if (ins[i][l] > outs[i][r]) {
                    --l;
                } else {
                    ++edges_count;
                    g[getId(i, ins[i][l])].push_back({getId(i, outs[i][r]), 0});
                    --r;
                }
            }
        }
    }

    struct Compare {
        // Custom comparator for priority queue
        bool operator()(const tuple<int, int>& a, const tuple<int, int>& b) const {
            if(get<1>(a) == get<1>(b)) return get<0>(a) < get<0>(b);
            return get<1>(a) > get<1>(b);
        }
    };

    // Compute the shortest time from a starting vertex and time
    vector<int> ComputingShortestTime(int x, int ta, int tw) {
        vector<int> t(n, INT_MAX); // Initialize shortest times to infinity
        t[x] = 0;

        priority_queue<tuple<int, int>, vector<tuple<int, int> >, Compare> q;
        vector<int> w(vertices_count, INT_MAX); // Weights for each vertex in the transformed graph
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            const int iu = getId(x, *it);
            q.push({iu, 0}); // Push starting vertex-time pairs into the queue
            w[iu] = 0;
        }

        // Dijkstra's algorithm on the transformed graph
        while(!q.empty()) {
            const auto [iu, wu] = q.top();
            q.pop();
            
            if(wu > w[iu]) continue;
            
            for(const auto& [iv, wv] : g[iu]) {
                if(gi[iv].second > tw) continue; // Ignore vertices outside the time window
                if(w[iv] <= w[iu] + wv) continue;
                
                w[iv] = w[iu] + wv;
                t[gi[iv].first] = min(t[gi[iv].first], w[iv]); // Update shortest time for the original vertex
                q.push({iv, w[iv]});
            }
        }

        return t;
    }
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

    // Get starting vertex, begin time and end time
    int x, ta, tw;
    cin >> x >> ta >> tw;
    --x;

    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v;
        g.push_back({u, v, t, d});
    }

    // Generate the transformed graph
    TransformingGraph::GenerateGraph(n, g);

    // Compute the shortest time for each vertex
    vector<int> f = TransformingGraph::ComputingShortestTime(x, ta, tw);

    // Output the shortest times
    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}