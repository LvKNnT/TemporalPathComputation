#include "graphUtil.h"

#include <vector>
using std::vector;

/*
    Algorithm 6
    Receives the number of nodes, the edges in stream presentation, the target x, and the time interval [ta, tw].
        This will compute the reverse-foremost time+path from all other nodes to x in O(|V| + |E|)
    Also reports the reverse-foremost time+path from any specified origin to x in O(path_length) (path length is no greater than |V|)
*/
class ReverseForemost {   
    int target;
    vector<int> time;
    vector<Edge> parent;
public:
    /* compute the reverse-foremost times and means to retrieve the path upon construction of the class instance */
    ReverseForemost(const int &n, const vector<Edge> &edges, int target, int ta, int tw) : target(target), time(n), parent(n) {
        fill(time.begin(), time.end(), INT_MIN);
        time[target] = tw;

        for (int edge_id = (int)edges.size() - 1; edge_id >= 0; edge_id--) {
            const Edge &e = edges[edge_id];

            // break the loop early because further edges in the stream depart too early and won't be used
            if (e.t < ta) break; 

            // if from node u, we can use this edge to go to v and still be able to reach target
            if (e.t + e.lambda <= time[e.v]) {
                if (e.t > time[e.u]) {
                    // update time[u] with the path u -> v -> ... -> x
                    time[e.u] = e.t;
                    parent[e.u] = e;
                }
            }
        }
    }
    
    /* return all the reverse-foremost times from each vertex */
    vector<int> getAllReverseForemostTime() const {
        return time;
    }

    /* returns the reverse-foremost time and the path as a list of edges, if there is no available path then time will be -INF*/
    std::pair<int, vector<Edge>> getReverseForemostPath(int source) const {
        if (time[source] == INT_MIN) { // no path
            return make_pair(INT_MIN, vector<Edge>());
        }
        vector<Edge> path;
        for (int node = source; node != target; node = parent[node].v) {
            path.push_back(parent[node]); // the edge outgoing from 'node'
        }
        return make_pair(time[source], path);
    }
};