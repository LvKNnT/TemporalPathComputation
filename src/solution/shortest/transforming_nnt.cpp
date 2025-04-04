#include <bits/stdc++.h>

using namespace std;

namespace TransformingGraph {
    int n;
    map<pair<int, int>, vector<tuple<int, int, int> > > g;
    map<int, vector<int>> ins, outs;

    void GenerateGraph(const int&n, const vector<tuple<int, int, int, int> >& old_g) {
        TransformingGraph::n = n;

        map<pair<int, int>, bool> has_in, has_out;

        for(const auto& [u, v, ta, tw] : old_g) {
            g[{u, ta}].push_back({v, ta + tw, tw});
            
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
                g[{i, ins[i][j - 1]}].push_back({i, ins[i][j], 0});
            }
            
            for(int j=1;j<outs[i].size();++j) {
                g[{i, outs[i][j - 1]}].push_back({i, outs[i][j], 0});
            }

            for(int l = ins[i].size() - 1, r = outs[i].size() - 1; l >= 0 && r >= 0; --r) {
                while(l >= 0 && ins[i][l] > outs[i][r]) --l;
                
                if(l >= 0) {
                    g[{i, ins[i][l]}].push_back({i, outs[i][r], 0});
                }
            }
        }
    }

    struct Compare {
        bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const {
            if(get<2>(a) == get<2>(b)) return get<0>(a) < get<0>(b);
            return get<2>(a) > get<2>(b);
        }
    };

    // Need transforming graph to work
    vector<int> ComputingShortestTime(int x, int ta, int tw) {
        vector<int> t(n, INT_MAX);
        t[x] = 0;

        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, Compare> q;
        map<pair<int, int>, int> w;
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            q.push({x, *it, 0});
            w[{x, *it}] = 0;
        }

        while(!q.empty()) {
            const auto [u, tu, wu] = q.top();
            q.pop();
            
            if(wu > w[{u, tu}]) continue;
            
            for(const auto& [v, tv, wv] : g[{u, tu}]) {
                if(tv > tw) continue;
                if(w.find({v, tv}) != w.end() && w[{v, tv}] <= w[{u, tu}] + wv) continue;

                w[{v, tv}] = w[{u, tu}] + wv;
                if(w[{v, tv}] < t[v]) {
                    t[v] = w[{v, tv}];
                }
                q.push({v, tv, w[{v, tv}]});
            }
        }

        return t;
    }

    vector<int> GetShortestTime(int x, int ta, int tw) {
        vector<int> t(n, INT_MAX);
        t[x] = 0;

        vector<int> p(n, -1);
        p[x] = x;

        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, Compare> q;
        map<pair<int, int>, int> w;
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            q.push({x, *it, 0});
            w[{x, *it}] = 0;
        }

        while(!q.empty()) {
            const auto [u, tu, wu] = q.top();
            q.pop();
            
            if(wu > w[{u, tu}]) continue;
            
            for(const auto& [v, tv, wv] : g[{u, tu}]) {
                if(tv > tw) continue;
                if(w.find({v, tv}) != w.end() && w[{v, tv}] <= w[{u, tu}] + wv) continue;

                w[{v, tv}] = w[{u, tu}] + wv;
                if(w[{v, tv}] < t[v]) {
                    p[v] = u;
                    t[v] = w[{v, tv}];
                }
                q.push({v, tv, w[{v, tv}]});
            }
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

    vector<int> f = TransformingGraph::ComputingShortestTime(x, ta, tw);
    // vector<int> path = TransformingGraph::GetShortestTime(x, ta, tw);

    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}