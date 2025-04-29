#include "graphUtil.h"

#include <algorithm>
#include <vector>
using std::vector;

/*
    Algorithm 1
    Receives the number of nodes, the edges in stream presentation, the source x, and the time interval [ta, tw].
        This will compute the foremost time+path from x to all other nodes in O(|V| + |E|)
    Also reports the foremost time+path from x to any specified destination in O(path_length) (path length is no greater than |V|)
*/
class Foremost {   
    int source;
    vector<int> time;
    vector<Edge> parent;
public:
    /* compute the foremost times and means to retrieve the path upon construction of the class instance */
    Foremost(const int &n, const vector<Edge> &edges, int source, int ta, int tw) : source(source), time(n), parent(n) {
        fill(time.begin(), time.end(), INT_MAX);
        time[source] = ta;

        for (const Edge &e : edges) {
            if (e.t + e.lambda <= tw && e.t >= time[e.u]) { // edge appears after u's arrival and arrive within [ta, tw]
                if (e.t + e.lambda < time[e.v]) { 
                    // updates time[v] with the path x -> ... -> u -> v
                    time[e.v] = e.t + e.lambda;
                    parent[e.v] = e;
                }
            } else if (e.t > tw) {
                // break the loop early because further edges in streaming presentation depart too late and won't be used
                break; 
            }
        }
    }

    /* return all the foremost times to each vertex */
    vector<int> getAllForemostTime() const {
        return time;
    }

    /* returns the foremost time and the path as a list of edges, if there is no available path then time will be INF */
    std::pair<int, vector<Edge>> getForemostPath(int target) const {
        if (time[target] == INT_MAX) { // no path
            return make_pair(INT_MAX, vector<Edge>());
        }
        vector<Edge> path;
        for (int node = target; node != source; node = parent[node].u) {
            path.push_back(parent[node]); // the edge leading to 'node'
        }
        reverse(path.begin(), path.end());
        return make_pair(time[target], path);
    }
};