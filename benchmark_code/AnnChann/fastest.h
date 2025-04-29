#include "graphUtil.h"

#include <vector>
using std::vector;

/*
    Algorithm 3
    Receives the number of nodes, the edges in stream presentation, the source x, and the time interval [ta, tw].
        This will compute the fastest time+path from x to all other nodes.
    Also reports the fastest time+path from x to any specified destination.
*/
class Fastest {   
    struct Path { // presents a possible path to a node
        int s, a; // starting time and arrival time of the path
        Edge e; // last edge used in the path
        Path() {}
        Path(int s, int a, Edge e) : s(s), a(a), e(e) {}
    };
    int source;
    vector<int> time;
    vector<int> arg_a;
    vector<vector<Path>> L;

    /* find the greatest i so that L[i].a <= t, or -1 if there is no such i */
    inline int find(const vector<Path> &L, int t) const {
        int i = (int)L.size() - 1;
        while (i >= 0 && L[i].a > t) --i;
        return i;
    }

    /* 
    try to add (s, a) into the list L
        does nothing if (s, a) is dominated by some element in L
        otherwise remove elemenst dominated by (s, a) and add (s, a)
    domination condition: X dominate Y if X.a <= Y.a and X.s >= Y.s
    */
    inline void add(vector<Path> &L, int s, int a, Edge e) {
        int i = find(L, a);
        if (i != -1 && L[i].s >= s) return; // (s, a) is dominated
        if (i == -1 || L[i].a < a) ++i;
        int j = i;
        while (j < L.size() && L[j].s <= s) ++j;
        // the elements in [i, j) are dominated so we remove them and add (s, a) in their place
        L.emplace(L.erase(L.begin() + i, L.begin() + j), s, a, e);
    }

public:
    /* compute the fastest times and means to retrieve the path upon construction of the class instance */
    Fastest(const int &n, const vector<Edge> &edges, int source, int ta, int tw) : source(source), time(n), arg_a(n), L(n) {
        fill(time.begin(), time.end(), INT_MAX);
        time[source] = 0;

        for (const Edge &e : edges) {
            if (e.t > tw) break; // break the loop early because further edges in the stream depart too late and won't be used
            if (e.t < ta || e.t + e.lambda > tw) continue; // edge is outside of required time interval
            if (e.v == source) continue; // an edge leading to source won't be used in the computation for any node
            
            if (e.u == source) {
                // since e.t is in sorted order, only need to check the latest span (instantaneous span)
                if (L[e.u].empty() || L[e.u].back().a != e.t) { 
                     L[e.u].emplace_back(e.t, e.t, e);
                }
            }

            // find the best path to u that can update v
            int i = find(L[e.u], e.t);
            if (i == -1) continue; // all paths to u arrive later than e.t so we can't use the edge e
            int s = L[e.u][i].s, a = e.t + e.lambda;

            // try to add (s, a) to L[v]
            add(L[e.v], s, a, e);
            
            // update time[v]
            if (a - s < time[e.v]) {
                time[e.v] = a - s;
                arg_a[e.v] = a;
            }
        }
    }
    
    /* return all the fastest times to each vertex */
    vector<int> getAllFastestTime() const {
        return time;
    }
    
    /* returns the fastest time and the path as a list of edges, if there is no available path then time will be INF */
    std::pair<int, vector<Edge>> getFastestPath(int target) const {
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