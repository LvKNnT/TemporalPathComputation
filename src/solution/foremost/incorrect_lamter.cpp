#include <bits/stdc++.h>

int main(void) {
	std::ios_base::sync_with_stdio(0); std::cin.tie(nullptr);
	int n, m; std::cin >> n >> m;
	int x, min, max; std::cin >> x >> min >> max;
	x -= 1;
	for (int i = 0; i < n; i += 1) {
		std::cout << (i == x ? min : -1) << " \n"[i + 1 == n];
	}
}
