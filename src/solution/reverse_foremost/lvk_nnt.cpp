#include <bits/stdc++.h>

using namespace std;

void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
}

// latest-departure time
vector<int> ComputingReverseForemostTime(const int& n, vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MIN);
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

// latest-departure path
vector<int> GetReverseForemostPath(const int& n, vector<tuple<int, int, int, int> >& g, const int& x, const int& ta, const int& tw) {
    vector<int> t(n, INT_MIN);
    t[x] = tw;

    vector<int> p(n, -1);
    p[x] = x;

    for(int i = g.size() - 1; i >= 0; --i) {
        const auto& [u, v, t_edge, w_edge] = g[i];

        if(t_edge >= ta) {
            if(t_edge + w_edge <= t[v]) {
                if(t_edge > t[u]) {
                    p[v] = u;
                    t[u] = t_edge;
                }
            }
        }
        else break;
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

    vector<int> f = ComputingReverseForemostTime(n, g, x, ta, tw);
    // vector<int> path = GetReverseForemostPath(n, g, x, ta, tw);

    for(int i=0; i<n; ++i) {
        cout << (f[i] == INT_MAX ? -1 : f[i]) << " \n"[i+1 == n];
    }
}