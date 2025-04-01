#pragma once

// #include <bits/stdc++.h>
#include "Hasher.h"
#include <map>

using namespace std;

namespace TransformingGraph {
    // source:
    // https://codeforces.com/blog/entry/62393
    // I just tweak to make it usable for pair<int, int>
    // need to make sure two var in pair<int, int> are both positive
    // the idea is simply shift the first int to left 32 bit to fit into a long long
    struct custom_hash {
        static uint64_t splitmix64(uint64_t x) {
            // http://xorshift.di.unimi.it/splitmix64.c
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }
    
        size_t operator()(const pair<int, int>& p) const {
            static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
            uint64_t x = static_cast<uint64_t>(p.first) << 32 + static_cast<uint64_t>(p.second);
            return splitmix64(x + FIXED_RANDOM);
        }
    };

    unordered_map<pair<int, int>, vector<tuple<int, int, int> >, custom_hash> g, rev_g;
    unordered_map<int, vector<int>> ins, outs;

    void GenerateGraph(const vector<tuple<int, int, int, int> >& old_g) {
        g.clear();
        rev_g.clear();
        ins.clear();
        outs.clear();

        unordered_map<pair<int, int>, bool, custom_hash> has_in, has_out;

        for(const auto& [u, v, ta, tw] : old_g) {
            g[{u, ta}].push_back({v, ta + tw, tw});
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
        
        for(int i=0;i<hasher.GetSize();++i) {
            sort(ins[i].begin(), ins[i].end());
            sort(outs[i].begin(), outs[i].end());

            for(int j=1;j<ins[i].size();++j) {
                g[{i, ins[i][j - 1]}].push_back({i, ins[i][j], 0});
                rev_g[{i, ins[i][j]}].push_back({i, ins[i][j - 1], 0});
            }
            
            for(int j=1;j<outs[i].size();++j) {
                g[{i, outs[i][j - 1]}].push_back({i, outs[i][j], 0});
                rev_g[{i, outs[i][j]}].push_back({i, outs[i][j - 1], 0});
            }

            for(int l = ins[i].size() - 1, r = outs[i].size() - 1; l >= 0 && r >= 0; --r) {
                while(l >= 0 && ins[i][l] > outs[i][r]) --l;
                
                if(l >= 0) {
                    g[{i, ins[i][l]}].push_back({i, outs[i][r], 0});
                    rev_g[{i, outs[i][r]}].push_back({i, ins[i][l], 0});
                }
            }
        }
    }

    void ClearGraph() {
        g.clear();
        rev_g.clear();
        ins.clear();
        outs.clear();
    }

    // Need transforming graph to work
    vector<int> ComputingForemostTime(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> t(n, INT_MAX);
        t[x] = ta;

        queue<tuple<int, int> > q;
        unordered_map<pair<int, int>, bool, custom_hash> vis;
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            q.push({x, *it});
            vis[{x, *it}] = true;
        }

        while(!q.empty()) {
            const auto& [u, tu] = q.front();
            
            for(const auto& [v, tv, wv] : g[{u, tu}]) {
                if(tv > tw) continue;
                if(vis.find({v, tv}) != vis.end()) continue;
                

                t[v] = min(t[v], tv);
                vis[{v, tv}] = true;
                q.push({v, tv});
            }

            q.pop();
        }

        return t;
    }
    
    // Need reverse transforming graph to work
    vector<int> ComputingReverseForemostTime(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> t(n, INT_MIN);
        t[x] = tw;

        queue<tuple<int, int> > q;
        unordered_map<pair<int, int>, bool, custom_hash> vis;
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

    // Need transforming graph to work
    vector<int> ComputingFastestPathDuration(int x, int ta, int tw) {
        int n = hasher.GetSize();

        // weight
        vector<int> t(n, INT_MAX);
        t[x] = 0;

        // path
        vector<int> pre(n, -1);
        pre[x] = x;

        queue<tuple<int, int> > q;
        unordered_map<pair<int, int>, bool, custom_hash> vis;
        for(vector<int>::iterator it = upper_bound(outs[x].begin(), outs[x].end(), tw); it != outs[x].begin();) {
            --it;
            q.push({x, *it});
            vis[{x, *it}] = true;

            int cur_ta = *it;
            while(!q.empty()) {
                const auto& [u, tu] = q.front();
                
                for(const auto& [v, tv, w_v] : g[{u, tu}]) {
                    if(tv > tw) continue;
                    if(vis.find({v, tv}) != vis.end()) continue;
                    
                    if(tv - cur_ta < t[v]) {
                        t[v] = tv - cur_ta;
                        pre[v] = u;
                    }
                    vis[{v, tv}] = true;
                    q.push({v, tv});
                }

                q.pop();
            }
        }
        return t;
    }

    struct Compare {
        bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const {
            if(get<2>(a) == get<2>(b)) return get<0>(a) < get<0>(b);
            return get<2>(a) > get<2>(b);
        }
    };

    // Need transforming graph to work
    vector<int> ComputingShortestPathDistance(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<int> t(n, INT_MAX);
        t[x] = 0;

        vector<int> pre(n, -1);
        pre[x] = x;

        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, Compare> q;
        unordered_map<pair<int, int>, int, custom_hash> w;
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
                    pre[v] = u;
                }
                q.push({v, tv, w[{v, tv}]});
            }
        }

        return t;
    }
}
