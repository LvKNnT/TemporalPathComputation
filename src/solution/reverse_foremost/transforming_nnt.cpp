#include <bits/stdc++.h>

using namespace std;

namespace TransformingGraph {
    int n;
    map<pair<int, int>, vector<tuple<int, int, int> > > rev_g;
    map<int, vector<int>> ins, outs;

    void GenerateGraph(const int&n, const vector<tuple<int, int, int, int> >& old_g) {
        TransformingGraph::n = n;

        map<pair<int, int>, bool> has_in, has_out;

        for(const auto& [u, v, ta, tw] : old_g) {
            rev_g[{v, ta + tw}].push_back({u, ta, tw});
            
            if(has_out.find({u, ta}) == has_out.end()) {
                outs[u].push_back(ta);
                has_out[{u, ta}] = true;
            }
            if(has_in.find({v, ta + tw}) == has_in.end()) {
                ins[v].push_back(ta + tw);
                has_in[{v, ta + tw}] = true;
            }
        }
        
        for(int i=0; i<n; ++i) {
            sort(ins[i].begin(), ins[i].end());
            sort(outs[i].begin(), outs[i].end());

            for(int j=1;j<ins[i].size();++j) {
                rev_g[{i, ins[i][j]}].push_back({i, ins[i][j - 1], 0});
            }
            
            for(int j=1;j<outs[i].size();++j) {
                rev_g[{i, outs[i][j]}].push_back({i, outs[i][j - 1], 0});
            }

            for(int l = ins[i].size() - 1, r = outs[i].size() - 1; l >= 0 && r >= 0; --r) {
                while(l >= 0 && ins[i][l] > outs[i][r]) --l;
                
                if(l >= 0) {
                    rev_g[{i, outs[i][r]}].push_back({i, ins[i][l], 0});
                }
            }
        }
    }

    // Need reverse transforming graph to work
    vector<int> ComputingReverseForemostTime(int x, int ta, int tw) {
        vector<int> t(n, INT_MIN);
        t[x] = tw;

        queue<tuple<int, int> > q;
        map<pair<int, int>, bool> vis;
        for(vector<int>::iterator it = lower_bound(ins[x].begin(), ins[x].end(), ta); it != ins[x].end() && *it < tw; ++it) {
            q.push({x, *it});
            vis[{x, *it}];
        }

        while(!q.empty()) {
            const auto& [u, tu] = q.front();
            
            for(const auto& [v, tv, wv] : rev_g[{u, tu}]) {
                if(tv < ta) continue;
                if(vis.find({v, tv}) != vis.end()) continue;
                
                t[v] = max(t[v], tv);
                vis[{v, tv}] = true;
                q.push({v, tv});
            }

            q.pop();
        }
        
        return t;
    }

    // Need reverse transforming graph to work
    vector<int> GetReverseForemostPath(int x, int ta, int tw) {
        vector<int> t(n, INT_MIN);
        t[x] = tw;

        vector<int> p(n, -1);
        p[x] = x;

        queue<tuple<int, int> > q;
        map<pair<int, int>, bool> vis;
        for(vector<int>::iterator it = lower_bound(ins[x].begin(), ins[x].end(), ta); it != ins[x].end() && *it < tw; ++it) {
            q.push({x, *it});
            vis[{x, *it}];
        }

        while(!q.empty()) {
            const auto& [u, tu] = q.front();
            
            for(const auto& [v, tv, wv] : rev_g[{u, tu}]) {
                if(tv < ta) continue;
                if(vis.find({v, tv}) != vis.end()) continue;
                
                if(tv < t[v]) {
                    p[v] = u;
                    t[v] = tv;
                }
                vis[{v, tv}] = true;
                q.push({v, tv});
            }

            q.pop();
        }
        
        return t;
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // get number of vertices and edges
    int n, m;
    cin >> n >> m;

    // the graph with 4 tuple: u (satarting vertex), v (ending vertex), ta(begin time), tw(length of path)
    vector<tuple<int, int, int, int> > g;

    // get starting vertex, begin time and end time
    int x, ta, tw;
    cin >> x >> ta >> tw;
    --x;

    for(int i=0; i<m; ++i) {
        int u, v, t, d;
        cin >> u >> v >> t >> d;
        --u, --v;
        g.push_back({u, v, t, d});
    }

    TransformingGraph::GenerateGraph(n, g);

    vector<int> f = TransformingGraph::ComputingReverseForemostTime(x, ta, tw);
    // vector<int> path = TransformingGraph::GetReverseForemostPath(x, ta, tw);

    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}