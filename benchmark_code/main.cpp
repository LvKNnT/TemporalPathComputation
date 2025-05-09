// #include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <functional>

#include <filesystem>
#include "StreamPresentation.h"
#include "TransformingGraph.h"

// for AnnChann's Code
#include "AnnChann/graphUtil.h"
#include "AnnChann/foremost.h"
#include "AnnChann/revforemost.h"
#include "AnnChann/fastest.h"
#include "AnnChann/shortest.h"

vector<Edge> edges;

// end of AnnChann's Code

using namespace std;
namespace fs = std::filesystem;

void getGraph(fstream& fin, fstream& fout) {
    fout << "Generating graph...\n";
    cerr << "Generating graph...\n";
    auto begin = std::chrono::high_resolution_clock::now();
    auto sbegin = std::chrono::high_resolution_clock::now();
    
    vector<tuple<int, int, int, int> > g;
    
    string line;
    int n, m;
    {
        // Process the first line
        getline(fin, line);
        stringstream ss(line);
        ss >> n >> m;
        hasher.AddFixedId(n);
    }
    for(int i=0;i<m;++i) {
        getline(fin, line);
        if(!line.empty() && line[0] == '%') continue; // skipping comment line
        
        stringstream ss(line);
        string u, v;
        int ta, tw;
        if((ss >> u >> v >> ta >> tw) && tw >= 0) {
            g.push_back({hasher.AddString(u), hasher.AddString(v), ta, tw});

            edges.push_back({hasher.AddString(u), hasher.AddString(v), ta, tw});
        }
        else {
            fout << "Invalid line!\n"; // should not come here
            cerr << "Invalid line!\n"; // should not come here
            return;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "getting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    cerr << "getting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    sbegin = std::chrono::high_resolution_clock::now();
    StreamPresentation::GenerateGraph(g);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "generating streaming graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "generating streaming graph: " << elapsed.count() * 1e-9 << " seconds.\n";     
    
    sbegin = std::chrono::high_resolution_clock::now();
    TransformingGraph::GenerateGraph(g);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";   
    
    sbegin = std::chrono::high_resolution_clock::now();
    streamPresentation_ize(edges);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "sorting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    cerr << "sorting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";     

    fout << "Vertice count: " << n << "\n";
    cerr << "Vertice count: " << n << "\n";
    fout << "Edge count: " << m << "\n";
    cerr << "Edge count: " << m << "\n";

    fout << "Stream vertice count: " << hasher.GetSize() << "\n";
    cerr << "Stream vertice count: " << hasher.GetSize() << "\n";
    fout << "Stream edges count: " << g.size() << "\n";
    cerr << "Stream edges count: " << g.size() << "\n";

    fout << "Transforming vertice count: " << TransformingGraph::vertices_count << "\n";
    cerr << "Transforming vertice count: " << TransformingGraph::vertices_count << "\n";
    fout << "Transforming edges count: " << TransformingGraph::edges_count << "\n";
    cerr << "Transforming edges count: " << TransformingGraph::edges_count << "\n";

    fout << "AnnChann vertice count: " << hasher.GetSize() << "\n";
    cerr << "AnnChann vertice count: " << hasher.GetSize() << "\n";
    fout << "AnnChann edges count: " << edges.size() << "\n";
    cerr << "AnnChann edges count: " << edges.size() << "\n";

    fout << "\n";

    return;

    // the rest is for debug purposes only
    fstream fg("../Data/g.out", ios::out);
    for(int i=0;i<g.size();++i) {
        fg << hasher.GetString(get<0>(StreamPresentation::g[i])) << " " << hasher.GetString(get<1>(StreamPresentation::g[i])) << " " << get<2>(StreamPresentation::g[i]) << " " << get<3>(StreamPresentation::g[i]) << "\n";
    }
    fg.close();

    fstream fe("../Data/e.out", ios::out);
    for(int i=0;i<edges.size();++i) {
        fe << hasher.GetString(edges[i].u) << " " << hasher.GetString(edges[i].v) << " " << edges[i].t << " " << edges[i].lambda << "\n";
    }
    fe.close();
}

void ClearGraph() {
    StreamPresentation::ClearGraph();
    TransformingGraph::ClearGraph();
    edges.clear();
    hasher.Clear();
}

void output_d(const string& path, const vector<int>& f, bool rev) {
    fstream fout(path, ios::out);

    map<int, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[stoi(hasher.GetString(i))] = f[i];
    }
    for(const pair<int, int>& v : mp) {
        if(rev) {
            fout << v.first << ": " << (v.second != INT_MIN ? v.second : -1) << "\n";
        }
        else {
            fout << v.first << ": " << (v.second != INT_MAX ? v.second : -1) << "\n";
        }
    }
    fout << "\n";

    fout.close();
}

void output_distance(const string& path, const string& target, const int& ta, const int& tw) {
    vector<int> f = StreamPresentation::ComputingForemostTime(hasher.GetId(target), ta, tw);
    output_d(path + "/foremost.txt", f, false);

    f = StreamPresentation::ComputingReverseForemostTime(hasher.GetId(target), ta, tw);
    output_d(path + "/reverseforemost.txt", f, false);

    f = StreamPresentation::ComputingFastestPathDuration(hasher.GetId(target), ta, tw);
    output_d(path + "/fastest.txt", f, false);

    f = StreamPresentation::ComputingShortestPathDistance(hasher.GetId(target), ta, tw);
    output_d(path + "/shortest.txt", f, false);
}

// Retrieve the path from the parent list in O(nlogn)
vector<vector<int> > ComputingPath(const int& n, const vector<int>& path) {
    vector<vector<int> > f(n, vector<int> (0));
    vector<int> visited(n, 0);

    function<void(int)> dfs = [&](int node) {
        if (visited[node]) return;
        visited[node] = 1;

        if (path[node] != -1 && node != path[node]) {
            dfs(path[node]);
            f[node] = f[path[node]];
        }
        f[node].push_back(node);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && path[i] != -1) {
            dfs(i);
        }
    }

    return f;
}

void output_p(const string& path, const vector<vector<int> >& f) {
    fstream fout(path, ios::out);

    map<int, vector<int> > mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[stoi(hasher.GetString(i))] = f[i];
    }
    for(const pair<int, vector<int> >& v : mp) {
        fout << v.first << ": ";
        for(int i = 0; i < v.second.size(); ++i) {
            fout << hasher.GetString(v.second[i]) << " ";
        }
        if(v.second.empty()) fout << "-1";
        fout << "\n";
    }
}

void output_path(const string& path, const string& target, const int& ta, const int& tw) {
    vector<int> f = StreamPresentation::GetForemostTimePath(hasher.GetId(target), ta, tw);
    vector<vector<int> > p = ComputingPath(hasher.GetSize(), f); 
    output_p(path + "/foremost.txt", p);

    f = StreamPresentation::GetReverseForemostPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/reverseforemost.txt", p);

    f = StreamPresentation::GetFastestPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/fastest.txt", p);

    f = StreamPresentation::GetShortestPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/shortest.txt", p);
}

// Show the difference between f and ff
void showDifference(fstream& fout, const vector<int>& f, const vector<int>& ff) {    
    map<string, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        if(f[i] != ff[i]) {
            mp[hasher.GetString(i)] = i;
        }
    }
    if(!mp.empty()) { 
        for(const auto& v : mp) {
            fout << v.first << ": " << f[v.second] << " " << ff[v.second] << "\n";
        }
        fout << "\n";
    }
}

bool checkForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout, const bool& isDebug) {
    if(isDebug) fout << "Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingForemostTime(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingForemostTime(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    // Solution provided by AnnChann's code
    begin = std::chrono::high_resolution_clock::now();
    
    Foremost fm(hasher.GetSize(), edges, des, ta, tw);
    vector<int> fff = fm.getAllForemostTime();
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "AnnChann's code: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff && f == fff) {
        if(isDebug) fout << "Correct!\n";
        return true;
    }

    fout << "False!\n";
    if(f != ff) {
        fout << "f != ff\n";
        if(isDebug) showDifference(fout, f, ff);
    }
    if(f != fff) {
        fout << "f != fff\n";
        if(isDebug) showDifference(fout, f, fff);
    }
    return false;
}

bool checkReverseForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout, const bool& isDebug) {
    if(isDebug) fout << "Reverse-Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingReverseForemostTime(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingReverseForemostTime(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    // Solution provided by AnnChann's code
    begin = std::chrono::high_resolution_clock::now();

    ReverseForemost rfm(hasher.GetSize(), edges, des, ta, tw);
    vector<int> fff = rfm.getAllReverseForemostTime();
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "AnnChann's code: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff && f == fff) {
        if(isDebug) fout << "Correct!\n";
        return true;
    }
    
    fout << "False!\n";
    if(f != ff) {
        fout << "f != ff\n";
        if(isDebug) showDifference(fout, f, ff);
    }
    if(f != fff) {
        fout << "f != fff\n";
        if(isDebug) showDifference(fout, f, fff);
    }
    return false;
}

bool checkFastest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout, const bool& isDebug) {
    if(isDebug) fout << "Fastest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingFastestPathDuration(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingFastestPathDuration(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    // Solution provided by AnnChann's code
    begin = std::chrono::high_resolution_clock::now();

    Fastest ft(hasher.GetSize(), edges, des, ta, tw);
    vector<int> fff = ft.getAllFastestTime();
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "AnnChann's code: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff && f == fff) {
        if(isDebug) fout << "Correct!\n";
        return true;
    }
    
    fout << "False!\n";
    if(f != ff) {
        fout << "f != ff\n";
        if(isDebug) showDifference(fout, f, ff);
    }
    if(f != fff) {
        fout << "f != fff\n";
        if(isDebug) showDifference(fout, f, fff);
    }
    return false;
}

bool checkShortest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout, const bool& isDebug) {
    if(isDebug) fout << "Shortest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingShortestPathDistance(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingShortestPathDistance(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    // Solution provided by AnnChann's code
    begin = std::chrono::high_resolution_clock::now();

    Shortest st(hasher.GetSize(), edges, des, ta, tw);
    vector<int> fff = st.getAllShortestTime();
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    if(isDebug) fout << "AnnChann's code: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff && f == fff) {
        if(isDebug) fout << "Correct!\n";
        return true;
    }
    
    fout << "False!\n";
    if(f != ff) {
        fout << "f != ff\n";
        if(isDebug) showDifference(fout, f, ff);
    }
    if(f != fff) {
        fout << "f != fff\n";
        if(isDebug) showDifference(fout, f, fff);
    }
    return false;
}

void testRandomNodesStreamGraph(int n, const int64_t& RNGcode, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    // RNG
    mt19937_64 RNG(RNGcode);
    auto rand = [&](const int& l, const int& r) -> int {
        return uniform_int_distribution<int>(l, r)(RNG);
    };
    
    // Check for random n inputs
    fout << "Average time for " << n << " random inputs:\n";

    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
    
    for(int i=0;i<n;++i) {
        const int target = rand(0, hasher.GetSize() - 1);
        const int ta = 0, tw = INT_MAX;
        
        // Streaming Graph
        auto startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingForemostTime(target, ta, tw);
        auto endRun = std::chrono::high_resolution_clock::now();
        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingReverseForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        reverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingFastestPathDuration(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        fastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingShortestPathDistance(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        shortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    }
    
    fout << "Streaming graph:\n";
    fout << "Foremost: " << foremostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << reverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << fastestTime / n << " seconds.\n";
    fout << "Shortest: " << shortestTime / n << " seconds.\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test Random Stream Graph time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Random Stream Graph time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";       
}

void testAtNode(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();
    
    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return;
    }

    fout << "Test time for:\n";
    fout << "Target: " << target << "\n";
    fout << "Time window: " << ta << " " << tw << "\n";

    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
    double tforemostTime = 0.0, treverseForemostTime = 0.0, tfastestTime = 0.0, tshortestTime = 0.0;
    double aforemostTime = 0.0, areverseForemostTime = 0.0, afastestTime = 0.0, ashortestTime = 0.0;
    
    // Streaming Graph
    auto startRun = std::chrono::high_resolution_clock::now();
    StreamPresentation::ComputingForemostTime(des, ta, tw);
    auto endRun = std::chrono::high_resolution_clock::now();
    foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    StreamPresentation::ComputingReverseForemostTime(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    reverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    StreamPresentation::ComputingFastestPathDuration(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    fastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    StreamPresentation::ComputingShortestPathDistance(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    shortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    // Transforming Graph
    startRun = std::chrono::high_resolution_clock::now();
    TransformingGraph::ComputingForemostTime(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    tforemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    TransformingGraph::ComputingReverseForemostTime(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    treverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    TransformingGraph::ComputingFastestPathDuration(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    tfastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    startRun = std::chrono::high_resolution_clock::now();
    TransformingGraph::ComputingShortestPathDistance(des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    tshortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

    // AnnChann's Code
    startRun = std::chrono::high_resolution_clock::now();
    Foremost fm(hasher.GetFixedId(), edges, des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    aforemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

    startRun = std::chrono::high_resolution_clock::now();
    ReverseForemost rfm(hasher.GetFixedId(), edges, des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    areverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

    startRun = std::chrono::high_resolution_clock::now();
    Fastest ft(hasher.GetFixedId(), edges, des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    afastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

    startRun = std::chrono::high_resolution_clock::now();
    Shortest st(hasher.GetFixedId(), edges, des, ta, tw);
    endRun = std::chrono::high_resolution_clock::now();
    ashortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    
    fout << "Streaming graph:\n";
    fout << "Foremost: " << foremostTime << " seconds.\n";
    fout << "Reverse-Foremost: " << reverseForemostTime << " seconds.\n";
    fout << "Fastest: " << fastestTime << " seconds.\n";
    fout << "Shortest: " << shortestTime << " seconds.\n";

    fout << "Transforming graph:\n";
    fout << "Foremost: " << tforemostTime << " seconds.\n";
    fout << "Reverse-Foremost: " << treverseForemostTime << " seconds.\n";
    fout << "Fastest: " << tfastestTime << " seconds.\n";
    fout << "Shortest: " << tshortestTime << " seconds.\n";

    fout << "AnnChann's code:\n";
    fout << "Foremost: " << aforemostTime << " seconds.\n";
    fout << "Reverse-Foremost: " << areverseForemostTime << " seconds.\n";
    fout << "Fastest: " << afastestTime << " seconds.\n";
    fout << "Shortest: " << ashortestTime << " seconds.\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";   
}

void testRandomNodes(int n, int64_t RNGcode, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    // RNG
    mt19937_64 RNG(RNGcode);
    auto rand = [&](const int& l, const int& r) -> int {
        return uniform_int_distribution<int64_t>(l, r)(RNG);
    };
    
    // Check for random 100 inputs
    fout << "Average time for " << n << " random inputs:\n";

    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
    double tforemostTime = 0.0, treverseForemostTime = 0.0, tfastestTime = 0.0, tshortestTime = 0.0;
    double aforemostTime = 0.0, areverseForemostTime = 0.0, afastestTime = 0.0, ashortestTime = 0.0;
    
    for(int i=0;i<n;++i) {
        const int target = rand(0, hasher.GetSize() - 1);
        const int ta = 0, tw = INT_MAX;
        
        // Streaming Graph
        auto startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingForemostTime(target, ta, tw);
        auto endRun = std::chrono::high_resolution_clock::now();
        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingReverseForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        reverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingFastestPathDuration(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        fastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingShortestPathDistance(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        shortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        // Transforming Graph
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tforemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingReverseForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        treverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingFastestPathDuration(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tfastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingShortestPathDistance(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tshortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

        // AnnChann's Code
        startRun = std::chrono::high_resolution_clock::now();
        Foremost fm(hasher.GetFixedId(), edges, target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        aforemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

        startRun = std::chrono::high_resolution_clock::now();
        ReverseForemost rfm(hasher.GetFixedId(), edges, target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        areverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

        startRun = std::chrono::high_resolution_clock::now();
        Fastest ft(hasher.GetFixedId(), edges, target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        afastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

        startRun = std::chrono::high_resolution_clock::now();
        Shortest st(hasher.GetFixedId(), edges, target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        ashortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    }
    
    fout << "Streaming graph:\n";
    fout << "Foremost: " << foremostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << reverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << fastestTime / n << " seconds.\n";
    fout << "Shortest: " << shortestTime / n << " seconds.\n";

    fout << "Transforming graph:\n";
    fout << "Foremost: " << tforemostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << treverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << tfastestTime / n << " seconds.\n";
    fout << "Shortest: " << tshortestTime / n << " seconds.\n";

    fout << "AnnChann's code:\n";
    fout << "Foremost: " << aforemostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << areverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << afastestTime / n << " seconds.\n";
    fout << "Shortest: " << ashortestTime / n << " seconds.\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";                
}

bool checkRandomNodes(int n, int64_t RNGcode, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    // RNG
    mt19937_64 RNG(RNGcode);
    auto rand = [&](const int& l, const int& r) -> int {
        return uniform_int_distribution<int64_t>(l, r)(RNG);
    };
    
    // Check for random 100 inputs
    fout << "Check for " << n << " random inputs:\n";

    bool yes = true;
    
    for(int i=0;i<n;++i) {
        const int idTarget = rand(0, hasher.GetSize() - 1);
        const string target = hasher.GetString(idTarget); // over-complex
        const int ta = 0, tw = INT_MAX;
        
        if(!checkForemost(target, ta, tw, fin, fout, false)) {
            yes = false;
            fout << target << " " << ta << " " << tw << "\n";
        }
        if(!checkReverseForemost(target, ta, tw, fin, fout, false)) {
            yes = false;
            fout << target << " " << ta << " " << tw << "\n";
        }
        if(!checkFastest(target, ta, tw, fin, fout, false)) {
            yes = false;
            fout << target << " " << ta << " " << tw << "\n";
        }
        if(!checkShortest(target, ta, tw, fin, fout, false)) {
            yes = false;
            fout << target << " " << ta << " " << tw << "\n";
        }
    }

    if(yes) {
        fout << "System succeed!\n";
        cerr << "System succeed!\n";
    }
    else {
        fout << "System fails!\n";
        cerr << "System fails!\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Check Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Check Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n"; 
    
    return yes;
}

void createLog(const string& OUT_PATH) {
    if(fs::file_size(OUT_PATH) == 0) {
        cerr << "The res.out file is empty!\n";
        return;
    }

    string logDir = "../log/";
    
    // Ensure the log directory exists
    if(!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }

    // Find the next available log file
    int logIndex = 0;
    string logPath;
    do {
        logPath = logDir + "log" + to_string(logIndex) + ".txt";
        logIndex++;
    }
    while(fs::exists(logPath)); // ensure the file name is unique

    try {
        fs::copy_file(OUT_PATH, logPath, fs::copy_options::overwrite_existing);
        cerr << "Log file is saved at: " << logPath << "\n";
    }
    catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

bool checkAccurate(const string& target, const int& ta, const int& tw, fstream& fin, fstream& fout, const bool& isDebug) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    bool yes = true;
    
    yes = yes && checkForemost(target, ta, tw, fin, fout, isDebug);
    yes = yes && checkReverseForemost(target, ta, tw, fin, fout, isDebug);
    yes = yes && checkFastest(target, ta, tw, fin, fout, isDebug);
    yes = yes && checkShortest(target, ta, tw, fin, fout, isDebug);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test Accurate time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Accurate time measured: " << elapsed.count() * 1e-9 << " seconds.\n";  
    fout << "\n";

    return yes;
}

// Test generator for themis
void testGenerator(fstream& ftest, fstream& fsolution, const int k) {
    // get node k of the graph
    const int node = (hasher.GetSize() - 1) * k;
    const int ta = 0, tw = INT_MAX;

    ftest << hasher.GetFixedId() << " " << StreamPresentation::g.size() << "\n";
    ftest << hasher.GetString(node) << " " << ta << " " << tw << "\n";

    for(const auto& [u, v, ta, tw] : StreamPresentation::g) {
        ftest << hasher.GetString(u) << " " << hasher.GetString(v) << " " << ta << " " << tw << "\n";
    }

    const int INF = 2e9;
    vector<int> f = StreamPresentation::ComputingReverseForemostTime(node, ta, tw);
    vector<int> ans(hasher.GetFixedId(), INT_MIN);
    for(int i = 0; i < hasher.GetSize(); ++i) {
        ans[stoi(hasher.GetString(i)) - 1] = f[i];
    }
    for(const int& v : ans) {
        fsolution  << (v >= -INF ? v : -1) << " ";
    }
}

signed main() {
    const std::string DISTANCE_PATH = "../lvk_solution/distance/";
    const std::string PATH_PATH = "../lvk_solution/path/";
    const std::string DATA_PATH = "../Data/";
    // const std::string DATA_PATH = "../Data/ca-cit-HepPh/";
    // const std::string DATA_PATH = "../Data/wikiconflict/";
    const std::string OUT_PATH = "../res/res.out";
    
    // better UI command line
    cerr << "Choose the operation:\n";
    cerr << "1. Check for accurate from random nodes\n";
    cerr << "2. Test for random nodes\n";
    cerr << "3. Get path and solution\n";
    cerr << "4. Check accruate at specific node\n";
    cerr << "5. Test at specific node\n";
    cerr << "Input your choice: ";

    int choice;
    cin >> choice;
    if(choice < 1 || choice > 5) {
        cerr << "Invalid choice!\n";
        return 1;
    }
    cerr << "You chose option " << choice << ".\n";



    int randomNode = 100;
    if(choice == 1 || choice == 2) {
        cerr << "Input the number of random nodes: ";
        cin >> randomNode;
    }
    
    int64_t RNGcode = 2703;
    char16_t* randomNodePtr = new char16_t;
    RNGcode = (int64_t) randomNodePtr;
    delete randomNodePtr;

    // Access to output
    // create the output directory if it doesn't exist
    if(!fs::exists("../res/")) {
        fs::create_directory("../res/");
    }
    fstream fout(OUT_PATH, ios::out);
    if(!fout.is_open()) {
        cerr << "Failed to open output file!\n";
        return 1;
    }
    
    if(choice == 1 || choice == 2) {
        fout << "Random nodes: " << randomNode << "\n";
        cerr << "Random nodes: " << randomNode << "\n";
        fout << "RNG code: " << RNGcode << "\n";
        cerr << "RNG code: " << RNGcode << "\n";

        fout << "\n";
    }

    string target = "1";
    int ta = 0, tw = INT_MAX;
    if(choice == 3 || choice == 4 || choice == 5) {
        cerr << "Input the target node: ";
        cin >> target;
        cerr << "Input the time window: ";
        cin >> ta >> tw;
        
        fout << "Target node: " << target << "\n";
        fout << "Time window: " << ta << " " << tw << "\n";
    }
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(DATA_PATH)) {
            if (entry.is_regular_file() && entry.path().filename().string().rfind("out", 0) == 0) {
                cerr << "Processing file: " << entry.path().filename() << "\n";
                if(choice == 1 || choice == 2) {
                    fout << "Processing file: " << entry.path().filename() << "\n";
                }
                
                fstream fin(entry.path(), ios::in);
                if(!fin.is_open()) {
                    cerr << "Failed to open file!\n";
                    continue;
                }  
                
                getGraph(fin, fout);

                // Empty graph check
                if(StreamPresentation::g.empty()) {
                    cerr << "Graph is empty!\n";
                    fout << "Graph is empty!\n";
                    
                    ClearGraph();
                    fin.close();
                    continue;
                }

                switch (choice) {
                    case 1:
                        // Check accurate for random nodes
                        if(checkAccurate(target, ta, tw, fin, fout, true)) {
                            checkRandomNodes(randomNode, RNGcode, fin, fout);
                        }

                        break;
                    case 2:
                        // Test for random nodes
                        if(checkAccurate(target, ta, tw, fin, fout, true)) {
                            testRandomNodes(randomNode, RNGcode, fin, fout);
                        }

                        break;
                    case 3:
                        // Output distance
                        output_distance(DISTANCE_PATH + entry.path().parent_path().filename().string(), target, ta, tw);
                        // Output path 
                        output_path(PATH_PATH + entry.path().parent_path().filename().string(), target, ta, tw);

                        break;
                    case 4:
                        // Check accurate for specific node
                        checkAccurate(target, ta, tw, fin, fout, true);

                        break;
                    case 5:
                        // Test for specific node
                        if(checkAccurate(target, ta, tw, fin, fout, true)) {
                            testAtNode(target, ta, tw, fin, fout);
                        }

                        break;
                    default: break;
                }
                
                ClearGraph();
                
                fin.close();
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }
        
    fout.close();
        
    createLog(OUT_PATH);
    
    return 0;
}