#include <bits/stdc++.h>
template <class T> using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <class X, class Y> inline bool minimise(X& a, Y b) { return a > b ? a = b, true : false; }

const int INF = 1E9 + 7;

int main(void) {
	std::ios_base::sync_with_stdio(0); std::cin.tie(nullptr);

	int n, m; std::cin >> n >> m;
	int x, t_min, t_max; std::cin >> x >> t_min >> t_max;
	x -= 1;
	std::vector<std::vector<std::tuple<int, int, int>>> g(n);
	std::vector<std::map<int, int>> dist(n);
	min_priority_queue<std::tuple<int, int, int>> Q;

	auto minimiseMap = [&] (int x, int t, int v) -> bool {
		if (dist[x].count(t))
			return minimise(dist[x][t], v);
		dist[x][t] = v;
		return true;
	};

	if (minimiseMap(x, t_min, 0)) {
		Q.emplace(0, t_min, x);
	}

	while (m--) {
		int u, v, t, d; std::cin >> u >> v >> t >> d;
		u -= 1, v -= 1;
		g[u].emplace_back(v, t, d);
		if (u == x and t >= t_min and t <= t_max) {
			if (minimiseMap(u, t, 0))
				Q.emplace(0, t, u);
		}
	}

	while (not Q.empty()) {
		int d, t, u; std::tie(d, t, u) = Q.top(); Q.pop();
		if (dist[u][t] != d) {
			continue;
		}
		for (const auto& [v, tt, dd] : g[u]) {
			if (t <= tt and tt + dd <= t_max) {
				if (minimiseMap(v, tt + dd, d + dd))
					Q.emplace(dist[v][tt + dd], tt + dd, v);
			}
		}
	}

	for (int i = 0; i < n; i += 1) {
		if (dist[i].empty()) {
			std::cout << -1;
		} else {
			int ans = INF;
			for (const auto& [t, tt] : dist[i]) {
				ans = std::min(ans, tt);
			}
			std::cout << ans;
		}
		std::cout << " \n"[i + 1 == n];
	}

	return 0;
}
