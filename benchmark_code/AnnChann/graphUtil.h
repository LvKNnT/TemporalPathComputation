/*
    specification of graph-related definitions and helper methods
*/

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cassert>

// we replace INF to correct value to match our constraint of lambda and t
// const int INF = 2e9;

struct Edge {
    int u, v, t, lambda;
};

/* reorder the edges in non-decreasing order of starting time of edges */
void streamPresentation_ize(std::vector<Edge> &edges) { 
    std::sort(edges.begin(), edges.end(), [&](const Edge &e1, const Edge &e2) {
        return e1.t < e2.t;
    });
}

std::mt19937 rng(163);
// std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int randint(int l, int r) {
    return std::uniform_int_distribution<int>(l, r)(rng);
}

/* generate a random temporal graph */
std::pair<int, std::vector<Edge>> randomGraph(int lower_n = 1, int upper_n = 20, int lower_m = 0, int upper_m = 30, int T = 100, int L = 20) {
    int n = randint(lower_n, upper_n);
    int m = randint(lower_m, upper_m);
    std::vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u = randint(1, n);
        int v = randint(1, n);
        int t = randint(0, T);
        int lambda = randint(1, L);
        edges.push_back(Edge{u, v, t, lambda});
    }
    return make_pair(n, edges);
}

/* load temporal graph from a file */
std::pair<int, std::vector<Edge>> loadGraph(std::filesystem::path filepath) {
    std::ifstream inp(filepath);
    if (!inp.is_open()) {
        std::cout << "Can't load file!\n";
        exit(0);
    }
    int n, m;
    inp >> n >> m;
    std::vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v, t, lambda;
        inp >> u >> v >> lambda >> t;
        assert(lambda < INT_MAX && t < INT_MAX && lambda > 0);
        edges.push_back({u, v, t, lambda});
    }
    inp.close();
    return make_pair(n, edges);
}

/* check if a path is a valid temporal path that is contained within a time interval and have specified source/target (any source/target if -1) */
bool isPathValid(const int &n, const std::vector<Edge> &edges, const std::vector<Edge> &path, int ta = 0, int tw = INT_MAX, int source = -1, int target = -1) {
    // check endpoints and timespan of path
    if (path.empty()) 
        return !(source != -1 && target != -1 && source != target);
    if (source != -1 && path.front().u != source) 
        return false;
    if (target != -1 && path.back().v != target) 
        return false;
    if (path.front().t < ta || path.back().t + path.back().lambda > tw) 
        return false;

    // check is path is a valid temporal path
    for (int i = 0; i + 1 < path.size(); i++) {
        const Edge &e1 = path[i];
        const Edge &e2 = path[i + 1];
        if (e1.v != e2.u) 
            return false;
        if (e1.t + e1.lambda > e2.t) 
            return false;
    }

    return true;
}

/* compute the time of one of the 4 types of temporal path. the function assumes path is valid of their structure, timespan, source and target (use isPathValid() above)*/
int computeTimeOfPath(const int &n, const std::vector<Edge> &edges, int ta, int tw, const std::vector<Edge> &path, std::string type) { 
    if (type == "FOREMOST") {
        if (path.empty()) return ta;
        return path.back().t + path.back().lambda;
    }
    if (type == "REVFOREMOST") {
        if (path.empty()) return tw;
        return path.front().t;
    }
    if (type == "FASTEST") {
        if (path.empty()) return 0;
        return path.back().t + path.back().lambda - path.front().t;
    }
    if (type == "SHORTEST") {
        int sum = 0;
        for (const Edge &e : path) {
            sum += e.lambda;
        }
        return sum;
    }
    std::cout << "Wrong type specified, possible choices: FOREMOST REVFOREMOST FASTEST SHORTEST\n";
    exit(-1);
    return -1;
}