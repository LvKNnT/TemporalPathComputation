#include "graphUtil.h"

#include <vector>
using std::vector;

/*
    Algorithm 7
    Receives the number of nodes, the edges in stream presentation, the source x, and the time interval [ta, tw].
        This will compute the shortest time+path from x to all other nodes.
    Also reports the shortest time+path from x to any specified destination.
*/
class Shortest {   
    struct Path { // presents a possible path to a node
        int d, a; // distance and arrival time of the path
        Edge e; // the last edge used in the path
        Path() {}
        Path(int d, int a, Edge e) : d(d), a(a), e(e) {}
    };
    int source;
    vector<int> time;
    vector<int> arg_a;
    vector<vector<Path>> L; 

    /* find the greatest i so that L[i].a <= t, or -1 if there is no such i */
    inline int find(const vector<Path> &L, int t) const {
        int i = (int)L.size() - 1;
        for (; i >= 0 && L[i].a > t; --i);
        return i;
    }

    /* 
    try to add (d, a) into the list L:
        does nothing if (d, a) is dominated by some element in L
        otherwise remove elemenst dominated by (d, a) and add (d, a)
    domination condition: X dominate Y if X.a <= Y.a and X.d <= Y.d
    */
    inline void add(vector<Path> &L, int d, int a, Edge e) {
        int i = find(L, a);
        if (i != -1 && L[i].d <= d) return; // (d, a) is dominated
        if (i == -1 || L[i].a < a) ++i;
        int j = i;
        for (; j < L.size() && L[j].d >= d; ++j);
        // the elements in [i, j) are dominated so we remove them and add (d, a) in their place
        L.emplace(L.erase(L.begin() + i, L.begin() + j), d, a, e);
    }

public:
    /* compute the shortest times and means to retrieve the path upon construction of the class instance */
    Shortest(const int &n, const vector<Edge> &edges, int source, int ta, int tw) : source(source), time(n), arg_a(n), L(n) {
        fill(time.begin(), time.end(), INT_MAX);
        time[source] = 0;

        for (const Edge &e : edges) {
            if (e.t > tw) break; // break the loop early because further edges in the stream depart too late and won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of required time interval
            if (e.v == source) continue; // an edge leading to source won't be used in the computation for any node
            
            if (e.u == source) {
                // since e.t is in sorted order and d = 0, the first t will dominate all 
                if (L[e.u].empty()) { 
                     L[e.u].emplace_back(0, e.t, e);
                }
            }

            // find the best path to u that can update v
            int i = find(L[e.u], e.t);
            if (i == -1) continue; // all paths to u arrive later than e.t so we can't use the edge e
            int d = L[e.u][i].d + e.lambda, a = e.t + e.lambda;

            // try to add (d, a) to L[v]
            add(L[e.v], d, a, e);
            
            // update time[v]
            if (d < time[e.v]) {
                time[e.v] = d;
                arg_a[e.v] = a;
            }
        }
    }
    
    /* return all the fastest times to each vertex */
    vector<int> getAllShortestTime() const {
        return time;
    }
    
    /* returns the shortest time and the path as a list of edges, if there is no available path then time will be INF */
    std::pair<int, vector<Edge>> getShortestPath(int target) const {
        if (time[target] == INT_MAX) { // no path
            return make_pair(INT_MAX, vector<Edge>());
        }

        vector<Edge> path;
        int t = arg_a[target];
        for (int node = target; node != source; ) {
            int i = find(L[node], t);
            path.push_back(L[node][i].e);
            t = L[node][i].e.t;
            node = L[node][i].e.u;
        } 

        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }
};