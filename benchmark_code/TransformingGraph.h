#pragma once

// #include <bits/stdc++.h>
#include "Hasher.h"
#include "assert.h"
#include <map>

using namespace std;

namespace TransformingGraph {
    int vertices_count = 0, edges_count = 0;
    vector<pair<int, int> > gi;
    vector<vector<tuple<int, int> > > g, rev_g;
    vector<vector<int>> ins, outs;

    int getId(int u, int ta) {
        auto it = lower_bound(gi.begin(), gi.end(), make_pair(u, ta));
        if(it != gi.end() && it->first == u && it->second == ta) {
            return it - gi.begin();
        }
        assert(false);
        return -1;
    }

    void GenerateGraph(const vector<tuple<int, int, int, int> >& old_g) {
        int n = hasher.GetSize();
        map<pair<int, int>, bool> has_in, has_out, has_i;

        ins.assign(n, vector<int>());
        outs.assign(n, vector<int>());

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
            for(const auto& p : has_i) {
                if(p.second) {
                    gi.push_back(p.first);
                    ++i;
                }
            }

            vertices_count = gi.size();
            g.assign(vertices_count, vector<tuple<int, int> >());
            rev_g.assign(vertices_count, vector<tuple<int, int> >());
        }

        for(const auto& [u, v, ta, tw] : old_g) {
            ++edges_count;
            g[getId(u, ta)].push_back({getId(v, ta + tw), tw});
            rev_g[getId(v, ta + tw)].push_back({getId(u, ta), tw});
        }
        
        for(int i=0;i<hasher.GetSize();++i) {
            sort(ins[i].begin(), ins[i].end());
            sort(outs[i].begin(), outs[i].end());

            for(int j=1;j<ins[i].size();++j) {
                ++edges_count;
                g[getId(i, ins[i][j - 1])].push_back({getId(i, ins[i][j]), 0});
                rev_g[getId(i, ins[i][j])].push_back({getId(i, ins[i][j - 1]), 0});
            }
            
            for(int j=1;j<outs[i].size();++j) {
                ++edges_count;
                g[getId(i, outs[i][j - 1])].push_back({getId(i, outs[i][j]), 0});
                rev_g[getId(i, outs[i][j])].push_back({getId(i, outs[i][j - 1]), 0});
            }

            int l = ins[i].size() - 1, r = outs[i].size() - 1;
            while (l >= 0 && r >= 0) {
                if (ins[i][l] > outs[i][r]) {
                    --l;
                } else {
                    ++edges_count;
                    g[getId(i, ins[i][l])].push_back({getId(i, outs[i][r]), 0});
                    rev_g[getId(i, outs[i][r])].push_back({getId(i, ins[i][l]), 0});
                    --r;
                }
            }
        }


    }

    void ClearGraph() {
        vertices_count = 0;
        edges_count = 0;
        gi.clear();
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

        queue<int> q;
        vector<bool> vis(vertices_count, false);
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            const int iu = getId(x, *it);
            q.push(iu);
            vis[iu] = true;
        }

        while(!q.empty()) {
            const int u = q.front();
            q.pop();
            
            for(const auto& [iv, wv] : g[u]) {
                if(gi[iv].second > tw) continue;
                if(vis[iv]) continue;
                
                t[gi[iv].first] = min(t[gi[iv].first], gi[iv].second);
                vis[iv] = true;
                q.push(iv);
            }
        }

        return t;
    }
    
    // Need reverse transforming graph to work
    vector<int> ComputingReverseForemostTime(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> t(n, INT_MIN);
        t[x] = tw;

        queue<int> q;
        vector<bool> vis(vertices_count, false);
        for(vector<int>::iterator it = lower_bound(ins[x].begin(), ins[x].end(), ta); it != ins[x].end() && *it < tw; ++it) {
            const int iu = getId(x, *it);
            q.push(iu);
            vis[iu] = true;
        }

        while(!q.empty()) {
            const int u = q.front();
            q.pop();
            
            for(const auto& [iv, wv] : rev_g[u]) {
                if(gi[iv].second < ta) continue;
                if(vis[iv]) continue;

                t[gi[iv].first] = max(t[gi[iv].first], gi[iv].second);
                vis[iv] = true;
                q.push(iv);
            }
        }
        
        return t;
    }

    // Need transforming graph to work
    vector<int> ComputingFastestPathDuration(int x, int ta, int tw) {
        int n = hasher.GetSize();

        // weight
        vector<int> t(n, INT_MAX);
        t[x] = 0;

        queue<int> q;
        vector<bool> vis(vertices_count, false);
        for(vector<int>::iterator it = upper_bound(outs[x].begin(), outs[x].end(), tw); it != outs[x].begin();) {
            --it;

            const int iu = getId(x, *it);
            q.push(iu);
            vis[iu] = true;

            int cur_ta = *it;
            while(!q.empty()) {
                const int u = q.front();
                q.pop();
                
                for(const auto& [iv, w_v] : g[u]) {
                    if(gi[iv].second > tw) continue;
                    if(vis[iv]) continue;
                    
                    t[gi[iv].first] = min(t[gi[iv].first], gi[iv].second - cur_ta);
                    vis[iv] = true;
                    q.push(iv);
                }
            }
        }
        return t;
    }

    struct Compare {
        bool operator()(const tuple<int, int>& a, const tuple<int, int>& b) const {
            if(get<1>(a) == get<1>(b)) return get<0>(a) < get<0>(b);
            return get<1>(a) > get<1>(b);
        }
    };

    // Need transforming graph to work
    vector<int> ComputingShortestPathDistance(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<int> t(n, INT_MAX);
        t[x] = 0;

        priority_queue<tuple<int, int>, vector<tuple<int, int> >, Compare> q;
        vector<int> w(vertices_count, INT_MAX);
        for(vector<int>::iterator it = lower_bound(outs[x].begin(), outs[x].end(), ta); it != outs[x].end() && *it <= tw; ++it) {
            const int iu = getId(x, *it);
            q.push({iu, 0});
            w[iu] = 0;
        }

        while(!q.empty()) {
            const auto [iu, wu] = q.top();
            q.pop();
            
            if(wu > w[iu]) continue;
            
            for(const auto& [iv, wv] : g[iu]) {
                if(gi[iv].second > tw) continue;
                if(w[iv] <= w[iu] + wv) continue;
                
                w[iv] = w[iu] + wv;
                t[gi[iv].first] = min(t[gi[iv].first], w[iv]);
                q.push({iv, w[iv]});
            }
        }

        return t;
    }
}