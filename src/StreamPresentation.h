#pragma once

// #include <bits/stdc++.h>
#include "Hasher.h"
#include <algorithm>
#include <map>
#include <climits>
#include <tuple>
#include <set>


using namespace std;

namespace StreamPresentation {
    vector<tuple<int, int, int, int> > g;

    void GenerateGraph(const vector<tuple<int, int, int, int> >& cur_g) {
        g = cur_g;

        stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
            return get<2>(a) < get<2>(b);
        });
    }

    void ClearGraph() {
        g.clear();
    }

    // earliest-arrival path
    vector<int> ComputingForemostTime(const int& x, const int& ta, const int& tw) {
        vector<int> t(hasher.GetSize(), INT_MAX);
        t[x] = ta;

        for (const auto& [u, v, t_edge, w_edge] : g) {
            if(t_edge + w_edge <= tw && t_edge >= t[u]) {
                if(t_edge + w_edge < t[v]) {
                    t[v] = t_edge + w_edge;
                }
            }
            else if(t_edge >= tw) {
                break;
            }
        }

        return t;
    }

    vector<int> GetForemostTimePath(const int& x, const int& ta, const int& tw) {
        vector<int> t(hasher.GetSize(), INT_MAX);
        t[x] = ta;

        vector<int> p(hasher.GetSize(), -1);
        p[x] = x;

        for (const auto& [u, v, t_edge, w_edge] : g) {
            if(t_edge + w_edge <= tw && t_edge >= t[u]) {
                if(t_edge + w_edge < t[v]) {
                    p[v] = u;
                    t[v] = t_edge + w_edge;
                }
            }
            else if(t_edge >= tw) {
                break;
            }
        }

        return p;
    }

    // latest-departure path
    vector<int> ComputingReverseForemostTime(const int& x, const int& ta, const int& tw) {
        vector<int> t(hasher.GetSize(), INT_MIN);
        t[x] = tw;

        for(int i = g.size() - 1; i >= 0; --i) {
            const auto& [u, v, t_edge, w_edge] = g[i];

            if(t_edge >= ta) {
                if(t_edge + w_edge <= t[v]) {
                    if(t_edge > t[u]) {
                        t[u] = t_edge;
                    }
                }
            }
            else break;
        }

        return t;
    }

    vector<int> GetReverseForemostPath(const int& x, const int& ta, const int& tw) {
        vector<int> t(hasher.GetSize(), INT_MIN);
        t[x] = tw;

        vector<int> p(hasher.GetSize(), -1);
        p[x] = x;

        for(int i = g.size() - 1; i >= 0; --i) {
            const auto& [u, v, t_edge, w_edge] = g[i];

            if(t_edge >= ta) {
                if(t_edge + w_edge <= t[v]) {
                    if(t_edge > t[u]) {
                        p[u] = v;
                        t[u] = t_edge;
                    }
                }
            }
            else break;
        }

        return p;
    }

    // fastest-path but multipasses
    vector<int> ComputingFastestPathDurationMultiPasses(const int& x, const int& ta, const int& tw) {
        int n = hasher.GetSize();
        vector<int> f(n, INT_MAX);
        f[x] = 0;

        for(const auto& [u, v, t_edge, w_edge] : g) {
            if(u == x && ta <= t_edge && t_edge + w_edge <= tw) {
                vector<int> t = ComputingForemostTime(x, t_edge, tw);

                for(int i=0;i<n;++i) {
                    if(t[i] != INT_MAX) f[i] = min(f[i], t[i] - t_edge);
                }
            }
            else if(t_edge >= tw) {
                break;
            }
        }

        return f;
    }

    class SortedList {
    private:
        vector<pair<int, int> > lst;
    
    public:
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

            if(ans == -1) return {-1, -1};
            return lst[ans];
        }

        bool IsDominatedFast(const pair<int, int>& u, const pair<int, int>& v) {
            return (u.first >= v.first && u.second <= v.second);
        }
        
        void AddFast(const pair<int, int>& a) {
            int i = lst.size() - 1;
            for(; i >= 0 && lst[i].first >= a.first; --i) {
                if(IsDominatedFast(lst[i], a)) {
                    return;
                }
            }
            lst.emplace(lst.begin() + i + 1, a);
            for(; i >= 0; --i) {
                if(IsDominatedFast(a, lst[i])) lst.erase(lst.begin() + i);
                else break;
            }
        }

        bool IsDominatedShort(const pair<int, int>& u, const pair<int, int>& v) {
            return (u.first <= v.first && u.second <= v.second);
        }

        void AddShort(const pair<int, int>& a) {
            int i = lst.size() - 1;
            for(; i >= 0 && lst[i].first <= a.first; --i) {
                if(IsDominatedShort(lst[i], a)) {
                    return;
                }
            }
            lst.emplace(lst.begin() + i + 1, a);
            for(; i >= 0; --i) {
                if(IsDominatedShort(a, lst[i])) lst.erase(lst.begin() + i);
                else break;
            }
        }
    };

    // fastest-path
    vector<int> ComputingFastestPathDuration(const int& x, const int& ta, const int& tw) {
        int n = hasher.GetSize();

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        vector<SortedList> L(n);

        for(const auto& [u, v, t_edge, w_edge] : g) {
            if(t_edge >= ta && t_edge + w_edge <= tw) {
                if(u == x) {
                    L[x].AddFast({t_edge, t_edge});
                }

                pair<int, int> cur = L[u].UpperBound(t_edge);
                if(cur.first == -1) continue;
                
                cur.second = t_edge + w_edge;

                L[v].AddFast(cur);
                
                if(cur.second - cur.first < f[v]) {
                    f[v] = cur.second - cur.first;
                }
            }
            else if(t_edge >= tw) {
                break;
            }
        }

        return f;
    }

    // shortest-path distance
    vector<int> ComputingShortestPathDistance(const int& x, const int& ta, const int& tw) {
        int n = hasher.GetSize();

        vector<SortedList> L(n);

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        for(const auto& [u, v, t_edge, w_edge] : g) {
            if(t_edge >= ta && t_edge + w_edge <= tw) {
                if(u == x) {
                    L[x].AddShort({0, t_edge});
                }

                pair<int, int> cur = L[u].UpperBound(t_edge);
                if(cur.first == -1) continue; // nothing suitable

                cur.first += w_edge;
                cur.second = t_edge + w_edge;

                L[v].AddShort(cur); // Including the comparison section

                if(cur.first < f[v]) {
                    f[v] = cur.first;
                }
            }
            else if(t_edge >= tw) {
                break;
            }
        }

        return f;
    }
}