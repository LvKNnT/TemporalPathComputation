#pragma once

#include <bits/stdc++.h>
#include "Hasher.h"

using namespace std;

namespace TransformingGraph {
    using mmmp = map<int, map<int, map<pair<int, int>, int> > >;
    using mmpit = map<int, map<pair<int, int>, int> >::iterator;
    mmmp g, rev_g;

    void GenerateGraph(const vector<vector<int> >& old_g) {
        g.clear();

        map<int, map<int, bool> > nodes;

        for(const vector<int>& e : old_g) {
            int u = e[0], v = e[1], ta = e[2], tw = e[3];

            g[u][ta][{v, ta + tw}] = tw;
            nodes[u][ta] = true;
            nodes[v][ta + tw] = true;
        }
        
        for(const pair<int, map<int, bool> >& node : nodes) {
            int u = node.first;
            for(map<int, bool>::const_iterator it = node.second.begin(); next(it) != node.second.end(); ++it) {
                g[u][it->first][{u, next(it)->first}] = 0;
            }
        }
    }

    void GenerateReverseGraph(const vector<vector<int> >& old_g) {
        rev_g.clear();

        map<int, map<int, bool> > nodes;

        for(const vector<int>& e : old_g) {
            int u = e[0], v = e[1], ta = e[2], tw = e[3];

            rev_g[v][ta + tw][{u, ta}] = tw;
            nodes[u][ta] = true;
            nodes[v][ta + tw] = true;
        }
        
        for(const pair<int, map<int, bool> >& node : nodes) {
            int u = node.first;
            for(map<int, bool>::const_iterator it = node.second.begin(); next(it) != node.second.end(); ++it) {
                rev_g[u][next(it)->first][{u, it->first}] = 0;
            }
        }
    }

    // Need transforming graph to work
    vector<int> ComputingForemostTime(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> t(n, INT_MAX);
        t[x] = ta;

        queue<pair<int, int> > q;
        map<int, map<int, bool> > vis;
        for(mmpit it = g[x].lower_bound(ta); it != g[x].end() && it->first <= tw; ++it) {
            q.push({x, it->first});
            vis[x][it->first] = true;
        }

        while(!q.empty()) {
            const pair<int, int> u = q.front();
            q.pop();

            for(const pair<pair<int, int>, int>& node : g[u.first][u.second]) {
                int v = node.first.first;
                int tv = node.first.second;

                if(tv > tw) continue;
                if(vis[v].find(tv) != vis[v].end()) continue;

                t[v] = min(t[v], tv);
                vis[v][tv] = true;
                q.push({v, tv});
            }
        }

        return t;
    }
    
    // Need reverse transforming graph to work
    vector<int> ComputingReverseForemostTime(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> t(n, INT_MIN);
        t[x] = tw;

        queue<pair<int, int> > q;
        unordered_map<int, unordered_map<int, bool> > vis;
        for(mmpit it = rev_g[x].lower_bound(ta); it != rev_g[x].end() && it->first <= tw; ++it) {
            q.push({x, it->first});
            vis[x][it->first] = true;
        }

        while(!q.empty()) {
            const pair<int, int> u = q.front();
            q.pop();

            for(const pair<pair<int, int>, int>& node : rev_g[u.first][u.second]) {
                int v = node.first.first;
                int tv = node.first.second;

                if(tv < ta) continue;
                if(vis[v].find(tv) != vis[v].end()) continue;
                
                t[v] = max(t[v], tv);
                vis[v][tv] = true;
                q.push({v, tv});
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

        // path
        vector<int> pre(n, -1);
        pre[x] = x;

        queue<pair<int, int> > q;
        unordered_map<int, unordered_map<int, bool> > vis;
        for(mmpit it = g[x].upper_bound(tw);it != g[x].begin() && ta <= it->first;) {
            --it;
            q.push({x, it->first});
            vis[x][it->first] = true;

            int cur_ta = it->first;
            while(!q.empty()) {
                const pair<int, int> u = q.front();
                q.pop();

                for(const pair<pair<int, int>, int>& node : g[u.first][u.second]) {
                    int v = node.first.first;
                    int tv = node.first.second;
                    
                    if(tv > tw) continue;
                    if(vis[v].find(tv) != vis[v].end()) continue;

                    if(tv - cur_ta < t[v]) {
                        t[v] = tv - cur_ta;
                        pre[v] = u.first;
                    }
                    vis[v][tv] = true;
                    q.push({v, tv});
                }
            }
        }
        return t;
    }

    struct Compare {
        bool operator()(const pair<pair<int, int>, int>& a, const pair<pair<int, int>, int>& b) const {
            if(a.second == b.second) return a.first.second > b.first.second; // for a better debug
            return a.second > b.second;
        }
    };

    // Need transforming graph to work
    vector<int> ComputingShortestPathDistance(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<int> t(n, INT_MAX);
        t[x] = 0;

        vector<int> pre(n, -1);
        pre[x] = x;

        priority_queue<pair<pair<int, int>, int>, vector<pair<pair<int, int>, int> >, Compare> q;
        map<int, map<int, int> > w;
        for(mmpit it = g[x].lower_bound(ta); it != g[x].end() && it->first <= tw; ++it) {            
            q.push({{x, it->first}, 0});
            w[x][it->first] = 0;
        }

        while(!q.empty()) {
            const pair<pair<int, int>, int> u_node = q.top();
            q.pop();

            pair<int, int> u = u_node.first;
            int u_w = u_node.second;
            
            if(u_w > w[u.first][u.second]) continue;
            
            for(const pair<pair<int, int>, int>& node : g[u.first][u.second]) {
                int v = node.first.first;
                int tv = node.first.second;

                if(tv > tw) continue;
                if(w[v].find(tv) != w[v].end() && w[v][tv] <= w[u.first][u.second] + node.second) continue;

                w[v][tv] = w[u.first][u.second] + node.second;
                if(w[v][tv] < t[v]) {
                    t[v] = w[v][tv];
                    pre[v] = u.first;
                }
                q.push({{v, tv}, w[v][tv]});
            }
        }

        return t;
    }
}
