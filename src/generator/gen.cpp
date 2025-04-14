#include <bits/stdc++.h>
#include "testlib.h"

const int MAX = 1000000000;

int wnextr(int l, int r, int b) {
	return rnd.wnext(r - l + 1, b) + l;
}

int main(int argc, char** argv) {
	registerGen(argc, argv, 1);

	int n = opt<int>("n", 1000000);
	int m = opt<int>("m", 2000000);
	int max_t = opt<int>("max_t", MAX);
	int bias_t = opt<int>("bias_t", 0);
	int max_d = opt<int>("max_d", MAX);
	std::string mode = opt<std::string>("mode", "random");

	int x = rnd.next(n);
	int t_min = rnd.wnext(max_t, -bias_t);
	int t_max = rnd.wnext(max_t, +bias_t);
	if (t_min > t_max) {
		std::swap(t_min, t_max);
	}

	assert(m >= n - 1);
	std::vector<int> parent(n);
	for (int i = 1; i < n; i += 1) {
		parent[i] = rnd.next(i);
	}
	std::vector<int> order(n); std::iota(order.begin(), order.end(), 0);
	shuffle(order.begin(), order.end());
	std::vector<std::tuple<int, int, int, int>> edges; edges.reserve(m);

	if (mode == "random") {
		for (int i = 1; i < n; i += 1) {
			int u = order[i];
			int v = order[parent[i]];
			int t = rnd.next(0, max_t);
			int d = rnd.next(1, max_d);
			if (rnd.next(2)) {
				std::swap(u, v);
			}
			edges.emplace_back(u, v, t, d);
		}
		while ((int) edges.size() < m) {
			int u = rnd.next(n);
			int v = rnd.next(n);
			while (u == v)
				v = rnd.next(n);
			int t = rnd.next(0, max_t);
			int d = rnd.next(1, max_d);
			edges.emplace_back(u, v, t, d);
		}
	}

	if (mode == "smart") {
		std::vector<std::vector<int>> g(n);
		for (int i = 1; i < n; i += 1) {
			int u = order[i];
			int v = order[parent[i]];
			g[u].emplace_back(v);
			g[v].emplace_back(u);
		}
		std::vector<int> ans(n); ans[x] = t_min;
		auto dfs = [&] (auto& dfs, int u, int p) -> void {
			for (int v : g[u]) if (v ^ p) {
				int t = wnextr(ans[u], std::max(ans[u], t_max), -bias_t);
				int d = rnd.next(std::max(1, std::min(max_d, t_max - t))) + 1;
				edges.emplace_back(u, v, t, d);
				ans[v] = t + d;
				dfs(dfs, v, u);
			}
		};
		dfs(dfs, x, x);
		while ((int) edges.size() < m) {
			int u = rnd.next(n);
			int v = rnd.next(n);
			while (u == v)
				v = rnd.next(n);
			if (ans[u] > ans[v]) {
				int t = rnd.wnext(max_t, -bias_t);
				int d = rnd.next(1, max_d);
				edges.emplace_back(u, v, t, d);
			} else {
				int t = rnd.wnext(ans[u], ans[v], -bias_t);
				int d = rnd.next(1, std::min(max_d, std::max(1, ans[v] - ans[u])));
				edges.emplace_back(u, v, t, d);
			}
		}
	}

	assert((int) edges.size() == m);
	shuffle(edges.begin(), edges.end());
	std::sort(edges.begin(), edges.end(),
		[&] (const auto& a, const auto& b) -> bool {
			return std::get<2>(a) < std::get<2>(b);
		}
	);

	std::cout << n << ' ' << m << '\n';
	std::cout << x + 1 << ' ' << t_min << ' ' << t_max << '\n';
	for (const auto& [u, v, t, d] : edges) {
		std::cout << u + 1 << ' ' << v + 1 << ' ' << t << ' ' << d << '\n';
	}

	return 0;
}
