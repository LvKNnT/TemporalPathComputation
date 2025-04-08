#include <bits/stdc++.h>
#include "testlib.h"

const int MAX_N = 1000000;
const int MAX_M = 2000000;
const int MAX_T = 1000000000;

int main(int argc, char** argv) {
	registerValidation(argc, argv);

	int n = inf.readInt(1, MAX_N);
	inf.readSpace();
	int m = inf.readInt(1, MAX_M);
	inf.readEoln();
	int x = inf.readInt(1, n);
	inf.readSpace();
	int t_min = inf.readInt(0, MAX_T);
	inf.readSpace();
	int t_max = inf.readInt(0, MAX_T);
	inf.readEoln();
	ensuref(t_max >= t_min, "Interval is ill-formed");
	int last_t = -MAX_T;
	std::vector<std::vector<int>> g(n);
	for (int i = 0; i < m; i += 1) {
		int u = inf.readInt(1, n);
		inf.readSpace();
		int v = inf.readInt(1, n);
		inf.readSpace();
		ensuref(u != v, "Edge traveling to itself");
		int t = inf.readInt(1, MAX_T);
		inf.readSpace();
		int d = inf.readInt(1, MAX_T);
		inf.readEoln();
		ensuref(t >= last_t, "Edges is not sorted");
		last_t = t;
		g[u - 1].push_back(v - 1);
		g[v - 1].push_back(u - 1);
	}
	inf.readEof();
	std::vector<int> mark(n); mark[0] = 1;
	std::queue<int> Q; Q.push(0);
	while (not Q.empty()) {
		int u = Q.front(); Q.pop();
		for (int v : g[u]) if (not mark[v]) {
			mark[v] = 1;
			Q.push(v);
		}
	}
	int cntMark = std::accumulate(mark.begin(), mark.end(), 0);
	ensuref(cntMark == n, "Graph is not connected");
}
