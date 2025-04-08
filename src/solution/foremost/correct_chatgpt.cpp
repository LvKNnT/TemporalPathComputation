#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;

struct Edge {
    int from, to;
    int time, duration;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int start;
    int talpha, tomega;
    cin >> start >> talpha >> tomega;

    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].from >> edges[i].to >> edges[i].time >> edges[i].duration;
    }

    // Arrival times initialized to INF
    vector<int> arrival_time(n + 1, INF);
    arrival_time[start] = talpha;

    // Min-heap: (arrival_time, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(talpha, start);

    int i = 0;
    while (!pq.empty()) {
        auto [curr_time, u] = pq.top(); pq.pop();

        // Process all edges starting from current position in sorted order
        while (i < m && edges[i].time < curr_time) {
            i++;
        }

        int j = i;
        while (j < m && edges[j].time <= tomega) {
            auto [from, to, t, d] = edges[j];
            if (from == u && t >= curr_time) {
                int arrival = t + d;
                if (arrival < arrival_time[to]) {
                    arrival_time[to] = arrival;
                    pq.emplace(arrival, to);
                }
            }
            ++j;
        }
    }

    for (int v = 1; v <= n; ++v) {
        if (arrival_time[v] == INF) cout << -1 << " ";
        else cout << arrival_time[v] << " ";
    }
    cout << "\n";

    return 0;
}
