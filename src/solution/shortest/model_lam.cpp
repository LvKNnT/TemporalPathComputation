#include <bits/stdc++.h>

const int INF = 1E9 + 7;

class MonotonicMap {
private:
	std::map<int, int> mapA;
	std::map<int, int> mapB;
public:
	MonotonicMap(void) {}
	~MonotonicMap(void) {}

	bool insert(int a, int b) {
		auto itA = mapA.find(a);

		if (itA == mapA.end()) {
			itA = mapA.emplace(a, b).first;
		} else {
			int old_b = itA->second;
			if (b >= itA->second) {
				return false;
			}
			mapB.erase(old_b);
			itA->second = b;
		}
		for (auto jt = std::next(itA); jt != mapA.end() and jt->second <= b; ) {
			mapB.erase(jt->second);
			jt = mapA.erase(jt);
		}
		while (itA != mapA.begin()) {
			auto kt = std::prev(itA);
			if (kt->second < b)
				break;
			mapB.erase(kt->second);
			mapA.erase(kt);
		}
		mapB[b] = a;
		return true;
	}

	std::pair<int,int> lower_bound(int x) {
		auto it = mapB.lower_bound(x);
		if (it == mapB.end()) {
			return {+1, +1};
		}
		return {-it->second, -it->first};
	}
};

int main(void) {
	std::ios_base::sync_with_stdio(0); std::cin.tie(nullptr);

	int n, m; std::cin >> n >> m;
	int x, t_min, t_max; std::cin >> x >> t_min >> t_max;
	x -= 1;
	std::vector<int> ans(n, INF); ans[x] = 0;
	std::vector<MonotonicMap> L(n);
	for (int i = 0; i < m; i += 1) {
		int u, v, t, l; std::cin >> u >> v >> t >> l;
		u -= 1, v -= 1;
		if (t >= t_min and t + l <= t_max) {
			if (u == x) {
				L[u].insert(-0, -t);
			}
		}
		int d, a;
		std::tie(d, a) = L[u].lower_bound(-t);
		if (u == x) {
		}
		assert(d <= a);
		if (d == +1 and a == +1)
			continue;
		d *= -1;
		a *= -1;
		d += l;
		a += l;
		L[v].insert(d, a);
		ans[v] = std::min(d, ans[v]);
		if (t >= t_max) {
			break;
		}
	}
	for (int i = 0; i < n; i += 1) {
		std::cout << (ans[i] == +INF ? -1 : ans[i]) << ' ';
	}

	return 0;
}
