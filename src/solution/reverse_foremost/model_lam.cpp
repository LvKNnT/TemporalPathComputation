#include <bits/stdc++.h>

const int INF = 2147483647;

int main(void) {
	std::ios_base::sync_with_stdio(0); std::cin.tie(nullptr);

	int N, M; std::cin >> N >> M;
	int x, t_min, t_max; std::cin >> x >> t_min >> t_max;
	x -= 1;
	std::vector<int> ans(N, -INF); ans[x] = t_max;
	std::vector<std::tuple<int, int, int, int>> edges;
	edges.reserve(M);
	for (int i = 0; i < M; i += 1) {
		int u, v, t, d; std::cin >> u >> v >> t >> d;
		u -= 1, d -= 1;
		edges.emplace_back(u, v, t, d);
	}
	std::reverse(edges.begin(), edges.end());
	for (int i = 0; i < M; i += 1) {
		auto [u, v, t, d] = edges[i];
		if (t < t_min) {
			break;
		}
		if (t + d <= ans[v]) {
			ans[u] = t;
		}
	}
	for (int i = 0; i < N; i += 1) {
		std::cout << (ans[i] == -INF ? -1 : ans[i]) << " \n"[i + 1 == N];
	}

	return 0;
}
