#include <bits/stdc++.h>

using namespace std;

namespace TransformingGraph {
    int n;
    int vertices_count = 0, edges_count = 0;
    vector<pair<int, int> > gi; // Stores unique (vertex, time) pairs
    vector<vector<tuple<int, int> > > g; // Adjacency list for the transformed graph
    vector<vector<int>> ins, outs; // Stores incoming and outgoing times for each vertex

    int getId(int u, int ta) {
        // Finds the index of the (vertex, time) pair in gi
        auto it = lower_bound(gi.begin(), gi.end(), make_pair(u, ta));
        if(it != gi.end() && it->first == u && it->second == ta) {
            return it - gi.begin();
        }
        assert(false); // Should not reach here if the pair exists
        return -1;
    }

    void GenerateGraph(const int& n, const vector<tuple<int, int, int, int> >& old_g) {
        TransformingGraph::n = n;
        map<pair<int, int>, bool> has_in, has_out, has_i;

        ins.assign(n, vector<int>()); // Initialize incoming times for each vertex
        outs.assign(n, vector<int>()); // Initialize outgoing times for each vertex

        // Process the input graph and populate ins, outs, and has_i
        for(const auto& [u, v, ta, tw] : old_g) {
            if(has_out.find({u, ta}) == has_out.end()) {
                outs[u].push_back(ta);
                has_out[{u, ta}] = true;
            }
            if(has_in.find({v, ta + tw}) == has_in.end()) {
                ins[v].push_back(ta + tw);
                has_in[{v, ta + tw}] = true;
            }

            has_i[{u, ta}] = true;
            has_i[{v, ta + tw}] = true;
        }

        {
            int i = 0;
            // Populate gi with unique (vertex, time) pairs
            for(const auto& p : has_i) {
                if(p.second) {
                    gi.push_back(p.first);
                    ++i;
                }
            }

            vertices_count = gi.size(); // Total number of unique (vertex, time) pairs
            g.assign(vertices_count, vector<tuple<int, int> >()); // Initialize adjacency list
        }

        // Add edges to the transformed graph based on the input graph
        for(const auto& [u, v, ta, tw] : old_g) {
            ++edges_count;
            g[getId(u, ta)].push_back({getId(v, ta + tw), tw});
        }
        
        // Add edges for consecutive incoming and outgoing times
        for(int i=0;i<n;++i) {
            sort(ins[i].begin(), ins[i].end());
            sort(outs[i].begin(), outs[i].end());

            for(int j=1;j<ins[i].size();++j) {
                ++edges_count;
                g[getId(i, ins[i][j - 1])].push_back({getId(i, ins[i][j]), 0});
            }
            
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

    // Computes the fastest time to reach each vertex from the starting vertex x
    vector<int> ComputingFastestTime(int x, int ta, int tw) {
        vector<int> t(n, INT_MAX); // Initialize times to infinity
        t[x] = 0;

        queue<int> q;
        vector<bool> vis(vertices_count, false);
        // Start BFS from all valid outgoing times of vertex x
        for(vector<int>::iterator it = upper_bound(outs[x].begin(), outs[x].end(), tw); it != outs[x].begin();) {
            --it;

            const int iu = getId(x, *it);
            q.push(iu);
            vis[iu] = true;

            int cur_ta = *it;
            while(!q.empty()) {
                const int u = q.front();
                q.pop();
                
                // Traverse neighbors and update times
                for(const auto& [iv, w_v] : g[u]) {
                    if(gi[iv].second > tw) continue; // Skip if time exceeds tw
                    if(vis[iv]) continue;
                    
                    t[gi[iv].first] = min(t[gi[iv].first], gi[iv].second - cur_ta);
                    vis[iv] = true;
                    q.push(iv);
                }
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

    // The graph with 4-tuple: u (starting vertex), v (ending vertex), ta (begin time), tw (length of path)
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

    // Generate the transformed graph
    TransformingGraph::GenerateGraph(n, g);

    // Compute the fastest time to each vertex
    vector<int> f = TransformingGraph::ComputingFastestTime(x, ta, tw);

    // Output the results
    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}