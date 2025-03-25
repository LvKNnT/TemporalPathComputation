#pragma once

// #include <bits/stdc++.h>
#include "hasher.h"
#include <algorithm>
#include <map>

using namespace std;

namespace StreamPresentation {
    vector<vector<int> > g;

    void GenerateGraph(const vector<vector<int> >& cur_g) {
        g.clear();

        StreamPresentation::g = cur_g;

        stable_sort(g.begin(), g.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[2] < b[2];
        });
    }

    // earliest-arrival path
    vector<int> ComputingForemostTime(int x, int ta, int tw) {
        vector<int> t(hasher.GetSize(), INT_MAX);
        t[x] = ta;

        for(const vector<int>& e : g) {
            if(e[2] + e[3] <= tw && e[2] >= t[e[0]]) {
                if(e[2] + e[3] < t[e[1]]) {
                    t[e[1]] = e[2] + e[3];
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return t;
    }

    vector<int> GetForemostTimePath(int x, int ta, int tw) {
        vector<int> t(hasher.GetSize(), INT_MAX);
        t[x] = ta;

        vector<int> p(hasher.GetSize(), -1);
        p[x] = x;

        for(const vector<int>& e : g) {
            if(e[2] + e[3] <= tw && e[2] >= t[e[0]]) {
                if(e[2] + e[3] < t[e[1]]) {
                    p[e[1]] = e[0];
                    t[e[1]] = e[2] + e[3];
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return p;
    }

    // latest-departure path
    vector<int> ComputingReverseForemostTime(int x, int ta, int tw) {
        vector<int> t(hasher.GetSize(), INT_MIN);
        t[x] = tw;

        for(int i = g.size() - 1; i >= 0; --i) {
            const vector<int>& e = g[i];

            if(e[2] >= ta) {
                if(e[2] + e[3] <= t[e[1]]) {
                    if(e[2] > t[e[0]]) {
                        t[e[0]] = e[2];
                    }
                }
            }
            else break;
        }

        return t;
    }

    vector<int> GetReverseForemostPath(int x, int ta, int tw) {
        vector<int> t(hasher.GetSize(), INT_MIN);
        t[x] = tw;

        vector<int> p(hasher.GetSize(), -1);
        p[x] = x;

        for(int i = g.size() - 1; i >= 0; --i) {
            const vector<int>& e = g[i];

            if(e[2] >= ta) {
                if(e[2] + e[3] <= t[e[1]]) {
                    if(e[2] > t[e[0]]) {
                        p[e[0]] = e[1];
                        t[e[0]] = e[2];
                    }
                }
            }
            else break;
        }

        return p;
    }

    // fastest-path but multipasses
    vector<int> ComputingFastestPathDurationMultiPasses(int x, int ta, int tw) {
        int n = hasher.GetSize();
        vector<int> f(n, INT_MAX);
        f[x] = 0;

        for(const vector<int>& e : g) {
            if(e[0] == x && ta <= e[2] && e[2] + e[3] <= tw) {
                vector<int> t = ComputingForemostTime(x, e[2], tw);

                for(int i=0;i<n;++i) {
                    if(t[i] != INT_MAX) f[i] = min(f[i], t[i] - e[2]);
                }
            }
        }

        return f;
    }

    struct Compare {
        bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
            return a.second < b.second || (a.second == b.second && a.first < b.first);
        }
    };

    class SortedList {
    private:
        map<int, int> first_to_second; //use for fastest
        map<int, int> second_to_first; //use for shortest

    public:
        void FastAdd(pair<int, int> a) {
            // nicely pick-up
            if(second_to_first.find(a.second) != second_to_first.end()) {
                first_to_second.erase(second_to_first[a.second]);

                a.first = max(a.first, second_to_first[a.second]);
            }

            if(first_to_second.find(a.first) != first_to_second.end()) {
                second_to_first.erase(first_to_second[a.first]);

                a.second = min(a.second, first_to_second[a.first]);
            }
        
            first_to_second[a.first] = a.second;
            second_to_first[a.second] = a.first;
        }

        void ShortAdd(pair<int, int> a) {
            // nicely pick-up
            if(second_to_first.find(a.second) != second_to_first.end()) {
                first_to_second.erase(second_to_first[a.second]);

                a.first = min(a.first, second_to_first[a.second]);
            }
        
            if(first_to_second.find(a.first) != first_to_second.end()) {
                second_to_first.erase(first_to_second[a.first]);

                a.second = min(a.second, first_to_second[a.first]);
            }

            first_to_second[a.first] = a.second;
            second_to_first[a.second] = a.first;
        }

        pair<int, int> LowerboundSecond(const int& a) {
            map<int, int>::iterator it = second_to_first.upper_bound(a);
            
            // if not found
            if(it == second_to_first.begin()) return {-1, -1};

            --it;
            return {it->second, it->first};
        }

        void RemoveFastDominatedElements() {
            priority_queue<int> pq;
            for(auto it = first_to_second.begin(); it != first_to_second.end(); ++it) {
                while(!pq.empty() && pq.top() >= it->second) {
                    first_to_second.erase(second_to_first[pq.top()]);
                    second_to_first.erase(pq.top());
                    pq.pop();
                }

                pq.push(it->second);
            }
        }

        void RemoveShortDominatedElements() {
            pair<int, int> cur = {INT_MIN, INT_MAX};
            for(map<int, int>::iterator it = first_to_second.begin(); it != first_to_second.end();) {
                if((cur.first < it->first && cur.second <= it->second)
                || (cur.first == it->first && cur.second < it->second)) {
                    second_to_first.erase(it->second);
                    it = first_to_second.erase(it);
                }
                else {
                    cur.first = it->first;
                    cur.second = it->second;
                    ++it;
                }
            }
        }

        void print() {
            for(const auto& v : first_to_second) {
                cout << v.first << " " << v.second << "\n";
            }
        }
    };

    // fastest-path
    vector<int> ComputingFastestPathDuration(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        vector<StreamPresentation::SortedList> L(n);

        for(const vector<int>& e : g) {
            if(e[2] >= ta && e[2] + e[3] <= tw) {
                if(e[0] == x) {
                    L[x].FastAdd({e[2], e[2]});
                }

                pair<int, int> cur = L[e[0]].LowerboundSecond(e[2]);
                if(cur.first == -1) continue; // nothing suitable
                
                cur.second = e[2] + e[3];
                
                L[e[1]].FastAdd(cur); // Including the comparison section
                
                L[e[1]].RemoveFastDominatedElements();
                
                if(cur.second - cur.first < f[e[1]]) {
                    f[e[1]] = cur.second - cur.first;
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return f;
    }

    vector<int> GetFastestPathPath(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        vector<int> p(n, -1);
        p[x] = x;

        vector<StreamPresentation::SortedList> L(n);

        for(const vector<int>& e : g) {
            if(e[2] >= ta && e[2] + e[3] <= tw) {
                if(e[0] == x) {
                    L[x].FastAdd({e[2], e[2]});
                }

                pair<int, int> cur = L[e[0]].LowerboundSecond(e[2]);
                if(cur.first == -1) continue; // nothing suitable
                
                cur.second = e[2] + e[3];
                
                L[e[1]].FastAdd(cur); // Including the comparison section
                
                L[e[1]].RemoveFastDominatedElements();
                
                if(cur.second - cur.first < f[e[1]]) {
                    f[e[1]] = cur.second - cur.first;
                    p[e[1]] = e[0];
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return p;
    }

    // shortest-path distance
    vector<int> ComputingShortestPathDistance(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<SortedList> L(n);

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        for(const vector<int>& e : g) {
            if(ta <= e[2] && e[2] + e[3] <= tw) {
                if(e[0] == x) {
                    L[x].ShortAdd({0, e[2]});
                }

                pair<int, int> cur = L[e[0]].LowerboundSecond(e[2]);
                if(cur.first == -1) continue; // nothing suitable

                cur.first += e[3];
                cur.second = e[2] + e[3];

                L[e[1]].ShortAdd(cur); // Including the comparison section

                L[e[1]].RemoveShortDominatedElements();

                if(cur.first < f[e[1]]) {
                    f[e[1]] = cur.first;
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return f;
    }

    vector<int> GetShortestPathPath(int x, int ta, int tw) {
        int n = hasher.GetSize();

        vector<SortedList> L(n);

        vector<int> f(n, INT_MAX);
        f[x] = 0;

        vector<int> p(n, -1);
        p[x] = x;

        for(const vector<int>& e : g) {
            if(ta <= e[2] && e[2] + e[3] <= tw) {
                if(e[0] == x) {
                    L[x].ShortAdd({0, e[2]});
                }

                pair<int, int> cur = L[e[0]].LowerboundSecond(e[2]);
                if(cur.first == -1) continue; // nothing suitable

                cur.first += e[3];
                cur.second = e[2] + e[3];

                L[e[1]].ShortAdd(cur); // Including the comparison section

                L[e[1]].RemoveShortDominatedElements();

                if(cur.first < f[e[1]]) {
                    f[e[1]] = cur.first;
                    p[e[1]] = e[0];
                }
            }
            else if(e[2] >= tw) {
                break;
            }
        }

        return p;
    }
}