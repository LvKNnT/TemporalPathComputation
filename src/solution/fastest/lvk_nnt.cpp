#include <bits/stdc++.h>

using namespace std;

void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

// earliest-arrival distance
vector<int> ComputingForemostTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MAX);
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

// fastest dítance but multipasses
vector<int> ComputingFastestTimeMultiPasses(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> f(n, INT_MAX);
    f[x] = 0;

    for(const auto& [u, v, t_edge, w_edge] : g) {
        if(u == x && ta <= t_edge && t_edge + w_edge <= tw) {
            vector<int> t = ComputingForemostTime(n, g, x, t_edge, tw);

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
};

// fastest distance
vector<int> ComputingFastestTime(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
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

// fastest path
vector<int> GetFastestPath(const int& n, const vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> f(n, INT_MAX);
    f[x] = 0;

    vector<int> p(n, -1);
    p[x] = x;

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
                p[v] = u;
                f[v] = cur.second - cur.first;
            }
        }
        else if(t_edge >= tw) {
            break;
        }
    }

    return p;
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

    GenerateGraph(g);

    vector<int> f = ComputingFastestTime(n, g, x, ta, tw);
    vector<int> path = GetFastestPath(n, g, x, ta, tw);

    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}