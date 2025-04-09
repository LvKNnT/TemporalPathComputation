#include <bits/stdc++.h>

using namespace std;

void GenerateGraph(vector<tuple<int, int, int, int> >& g) {
    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });
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

// Retrieve the path from the parent list in O(nlogn)
vector<vector<int> > ComputingPath(const int& n, const vector<int>& path) {
    vector<vector<int> > f(n, vector<int> (0));
    vector<int> visited(n, 0);

    function<void(int)> dfs = [&](int node) {
        if (visited[node]) return;
        visited[node] = 1;

        if (path[node] != -1 && node != path[node]) {
            dfs(path[node]);
            f[node] = f[path[node]];
        }
        f[node].push_back(node);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && path[i] != -1) {
            dfs(i);
        }
    }

    return f;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // get number of vertices and edges
    int n, m;
    cin >> n >> m;

    // the graph with 4 tuple: u (starting vertex), v (ending vertex), ta(begin time), tw(length of path)
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

    vector<int> path = GetReverseForemostPath(n, g, x, ta, tw);
    vector<vector<int> > f = ComputingPath(n, path);

    for(int i=0; i<n; ++i) {
        if(f[i].empty()) {
            cout << "-1\n";
            continue;
        }

        cout << f[i].size() << " ";
        for(int j=0; j<f[i].size(); ++j) {
            cout << f[i][j] << " \n"[j+1 == f[i].size()];
        }
    }
}