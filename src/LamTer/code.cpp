#include <bits/stdc++.h>

template <class X, class Y>
inline bool minimise(X &a, Y b) {
	return b < a ? a = b, true : false;
}

template <class X, class Y>
inline bool maximise(X &a, Y b) {
	return b > a ? a = b, true : false;
}

const int INF = 1E9 + 7;

class Edge {
public:
	int from, to, start, time;
	Edge(int _u = 0, int _v = 0, int _s = 0, int _t = 0): from(_u), to(_v), start(_s), time(_t) {}
};

class EdgeStream {
private:
	int m;
	std::vector<Edge> edges;
public:
	EdgeStream(void) {}
	EdgeStream(int _m): m(_m) {
		edges.reserve(m);
	}

	void add_edge(int u, int v, int s, int t) {
		edges.emplace_back(u, v, s, t);
	}

	void for_edge(const std::function<bool(int, int, int, int)>& f) const {
		for (const auto& e : edges) {
			if (f(e.from, e.to, e.start, e.time))
				break;
		}
	}
};

bool is_dominate(int a, int s, int aa, int ss) {
	return (s > ss and a <= aa) or (s == ss and a < aa);
}

void remove_dominate_elements_forward(std::set<std::pair<int, int>>::iterator it, std::set<std::pair<int, int>>& s) {
	auto current = it;
	current = std::next(current);
	while (current != s.end()) {
		if (is_dominate(it->first, it->second, current->first, current->second)) {
			current = s.erase(current);
		} else {
			return;
		}
	}
}

void remove_dominate_elements_backward(std::set<std::pair<int, int>>::iterator it, std::set<std::pair<int, int>>& s) {
	auto current = it;
	if (current == s.begin())
		return;
	current = std::prev(current);
	while (current != s.begin()) {
		if (is_dominate(it->first, it->second, current->first, current->second)) {
			current = s.erase(current);
			current = std::prev(current);
		} else {
			return;
		}
	}
}

void remove_dominate_elements(std::set<std::pair<int, int>>::iterator it, std::set<std::pair<int, int>>& s) {
	remove_dominate_elements_forward(it, s);
	remove_dominate_elements_backward(it, s);
}

class Graph {
private:
	int n;
	EdgeStream stream;
public:
	Graph(void) {}
	Graph(int _n, EdgeStream _stream): n(_n), stream(_stream) {}

	std::vector<int> compute_foremost_time(int x, int ta, int tb) const {
		std::vector<int> ans(n, INF);
		ans[x] = ta;
		stream.for_edge(
			[&] (int u, int v, int s, int t) -> bool {
				if (s + t <= tb and s >= ans[u]) {
					minimise(ans[v], s + t);
				}
				if (s >= tb)
					return true;
				return false;
			}
		);
		return ans;
	}

	std::vector<int> compute_fastest_path_duration_multiple_pass(int x, int ta, int tb) const {
		std::vector<int> ans(n, INF);
		ans[x] = 0;
		std::set<int> S;
		stream.for_edge(
			[&] (int u, int v, int s, int t) -> bool {
				if (v == x and s >= ta and s + t <= tb)
					S.insert(s);
				return false;
			}
		);
		for (int t : S) {
			std::vector<int> cans = compute_foremost_time(x, t, tb);
			for (int i = 0; i < n; i += 1) {
				minimise(ans[i], cans[i] - t);
			}
		}
		return ans;
	}

	std::vector<int> compute_fastest_path_duration_one_pass(int x, int ta, int tb) const {
		std::vector<int> ans(n, INF);
		ans[x] = 0;
		std::vector<std::map<int, int>> start_to_arrive(n);
		std::vector<std::set<std::pair<int, int>>> L(n);
		stream.for_edge(
			[&] (int u, int v, int s, int t) -> bool {
				if (s >= ta and s + t <= tb) {
					if (u == x) {
						L[x].insert({s, s});
						start_to_arrive[x][s] = s;
					}
					auto it = L[u].upper_bound({t, INF});
					if (it == L[u].begin()) {
						return false;
					}
					it = std::prev(it);
					int start_v = it->second;
					int arrive_v = s + t;
					if (start_to_arrive[v].count(start_v)) {
						L[v].erase({start_to_arrive[v][start_v], start_v});
						assert(start_to_arrive[v][start_v] >= arrive_v);
						start_to_arrive[v][start_v] = arrive_v;
					}
					auto insert_return_value = L[v].insert({arrive_v, start_v});
					assert(insert_return_value.second);
					it = insert_return_value.first;
					remove_dominate_elements(it, L[v]);
					minimise(ans[v], arrive_v - start_v);
				}
				if (s >= tb)
					return true;
			}
		);
		return ans;
	}
};

int main(int argc, char** argv) {
	std::ios_base::sync_with_stdio(0); std::cin.tie(nullptr);

	EdgeStream stream;
	// Input section
	std::unordered_set<int> vertices;
	std::fstream fin("../../raw/ca-cit-HepPh/out.ca-cit-HepPh", std::ios::in);

	std::string line;
    while(getline(fin, line)) {
        if(!line.empty() && line[0] == '%') continue; // skipping comment line
        
        std::stringstream ss(line);
		int u, v, s, e;
        if(ss >> u >> v >> e >> s) {
			u -= 1, v -= 1;
			stream.add_edge(u, v, s, e);
			vertices.insert(u);
			vertices.insert(v);
        }
    }
	fin.close();
	// End input

	Graph G(vertices.size(), stream);

	std::vector<int> t = G.compute_foremost_time(0, 0, INT_MAX);

	// Output section
	std::fstream fout("../../res/res.out", std::ios::out);
	for(int i=0;i<vertices.size();++i) {
		fout << i+1 << ": " << t[i] << "\n";
	}
	fout.close();
	// End output

	return 0;
}
